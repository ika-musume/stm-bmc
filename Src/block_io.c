#include <string.h>
#include "block_io.h"
#include "bmc.h"
#include "sequencer.h"
#include "util.h"
#include "console.h"

/* Rotate each redundant copy of a sector by this many bits, to avoid a situation where one extra
 * defective loop causes the same bit in all copies to be currupt
 */
const int sector_shifts[SECTOR_REDUNDANCY] = {0, 6, 12, 3, 9, 2, 7};

/* Calculate and insert a CRC into a block buffer. Block buffers are the same capacity as sector
 * buffers, but blocks are shorter. A sector buffer has enough space for the block contents, CRC,
 * and the bits corresponding to defective loops.
 */
static void insert_crc(uint8_t *block_buf)
{
    uint16_t crc = crc16(block_buf, BLOCK_LEN);
    block_buf[SERCTOR_CRC_OFFSET] = crc & 0xff;
    block_buf[SERCTOR_CRC_OFFSET + 1] = crc >> 8;
    block_buf[SERCTOR_CRC_OFFSET + 2] = 0;
}

/* 0 means success */
static int verify_crc(uint8_t *block_buf)
{
    uint16_t crc_actual = crc16(block_buf, BLOCK_LEN);
    uint16_t crc_stored = (block_buf[SERCTOR_CRC_OFFSET]) | (block_buf[SERCTOR_CRC_OFFSET + 1] << 8);

    return crc_stored != crc_actual;
}

/* Read a given sector and check CRC */
int bmc_read_sector(int loop_pos, uint8_t *buf)
{
    bmc_read_sector_raw(loop_pos, buf, SECTOR_LEN * 8);

    return verify_crc(buf);;
}

/* Write a given block buffer to a given sector, inserting a CRC in the process */
int bmc_write_sector(int loop_pos, uint8_t *buf)
{
    insert_crc(buf);
    return bmc_write_sector_raw(loop_pos, buf, SECTOR_LEN * 8);
}

/* Rotate a sector buffer by an arbitrary number of bits, through the entire
 * length of the sector
 */
static void shift_sector(uint8_t *buf, int amount)
{
    uint8_t output_buf[SECTOR_LEN];
    int i, j = amount;

    for (i = 0; i < SECTOR_LEN; i++) {
        if (j >= SECTOR_LEN)
            j -= SECTOR_LEN;

        if (j < 0)
            j += SECTOR_LEN;

        output_buf[j] = buf[i];

        j++;
    }

    memcpy(buf, output_buf, sizeof(output_buf));
}

/* Determine the the sector address of a particular redundant copy of a given block number */
static int sector_index(int block_num, int num_copy)
{
    return START_SECTOR + block_num * SECTOR_REDUNDANCY + num_copy;
}

/* Write a block buffer to the given redundant copies of the sector(s) corresponding to a given
 * block number.
 */
static int restore_sectors(int block_num, const uint8_t *block_buf, int redundancy_mask)
{
    int i, ret = 0, retries, write_result;

    uint8_t sector_buf[SECTOR_LEN];
    uint8_t scratch_buf[SECTOR_LEN];
    memset(sector_buf, 0, sizeof(sector_buf));

    for (i = 0; i < SECTOR_REDUNDANCY; i++) {
        if (redundancy_mask & BIT(i)) {
            memcpy(sector_buf, block_buf, BLOCK_LEN);
            insert_crc(sector_buf);
            shift_sector(sector_buf, sector_shifts[i]);

            retries = 0;

            do {
                write_result = bmc_write_sector_raw(sector_index(block_num, i), sector_buf, SECTOR_LEN * 8);

                if (write_result) {
                    retries++;
                    // Erase sector before attempting rewrite, otherwise things will collide
                    bmc_read_sector_raw(sector_index(block_num, i), scratch_buf, SECTOR_LEN * 8);
                }
            } while (write_result && retries < 20);

            if (retries)
                uart_printf("Warning: write to sector %d took %d retries\n", sector_index(block_num, i), retries);

            ret += write_result;
        }
    }

    return ret;
}

/* Erase all sectors corresponding to a given block */
void block_erase(int block_num)
{
    uint8_t sector_buf[SECTOR_LEN];
    int i;

    for (i = 0; i < SECTOR_REDUNDANCY; i++) {
        bmc_read_sector_raw(sector_index(block_num, i), sector_buf, SECTOR_LEN * 8);
    }
}

/* Write a block buffer to a given block. The block must be erased separately. */
int block_write(int block_num, const uint8_t *block_buf)
{
    if (restore_sectors(block_num, block_buf, -1))
        return restore_sectors(block_num, block_buf, -1);
    return 0;
}

/* Combine the results of two reads of the same sector, in case the transfer gate is glitchy */
static void combine_sector_buffers(uint8_t *dest, uint8_t *src)
{
    int i;
    for (i = 0; i < SECTOR_LEN; i++)
        dest[i] |= src[i];
}

/* Non-destructively read a given block, optionally counting the number of retries */
int block_read(int block_num, uint8_t *block_buf, int *error_count)
{
    int read_mask = 0;
    int attempt = 0;
    int success = 0;
    int ret;

    uint8_t sector_buf[SECTOR_LEN];
    uint8_t sector_buf_retry[SECTOR_LEN];
    uint8_t combined_buf[SECTOR_LEN];

    memset(combined_buf, 0, sizeof(combined_buf));

    while (!success && attempt < SECTOR_REDUNDANCY) {

        /* Keep track of which copies need to be restored */
        read_mask |= BIT(attempt);

        memset(sector_buf, 0, sizeof(sector_buf));
        bmc_read_sector_raw(sector_index(block_num, attempt), sector_buf, SECTOR_LEN * 8);
        shift_sector(sector_buf, -sector_shifts[attempt]);

        if (verify_crc(sector_buf) == 0) {
            success = 1;
            memcpy(combined_buf, sector_buf, SECTOR_LEN);
            break;
        }

        uart_printf("First read of block %d try %d:  ", block_num, attempt);
        dump_buffer(sector_buf, sizeof(sector_buf));

        memset(sector_buf_retry, 0, sizeof(sector_buf_retry));
        bmc_read_sector_raw(sector_index(block_num, attempt), sector_buf_retry, SECTOR_LEN * 8);
        shift_sector(sector_buf_retry, -sector_shifts[attempt]);

        uart_printf("Second read of block %d try %d: ", block_num, attempt);
        dump_buffer(sector_buf_retry, sizeof(sector_buf_retry));
        combine_sector_buffers(sector_buf_retry, sector_buf);
        uart_printf("Combined both reads:     ");
        dump_buffer(sector_buf_retry, sizeof(sector_buf_retry));

        if (verify_crc(sector_buf_retry) == 0) {
            uart_printf("Doing two reads of the same sector worked????????\n");
            success = 1;
            memcpy(combined_buf, sector_buf_retry, SECTOR_LEN);
            break;
        }

        uart_printf("\nError while reading block %d, copy %d\n", block_num, attempt);
        uart_printf("Read buffer: ");
        dump_buffer(sector_buf, SECTOR_LEN);
        uart_printf("\n");

        combine_sector_buffers(combined_buf, sector_buf);
        if (verify_crc(combined_buf) == 0) {
            success = 1;
            break;
        }

        uart_printf("Error in combined block %d, copy %d\n", block_num, attempt);
        uart_printf("Comb buffer: ");
        dump_buffer(combined_buf, SECTOR_LEN);

        attempt++;

        uart_printf("Warning: CRC error while reading block %d; now trying backup copy %d\n", block_num, attempt);

        purge_major_loop();
        purge_major_loop();
        purge_major_loop();
    }

    if (error_count)
        *error_count += attempt;

    if (!success) {
        uart_printf("Unrecoverable error while reading block %d\n", block_num);
        return -1;
    }

    memcpy(block_buf, combined_buf, BLOCK_LEN);

    ret = restore_sectors(block_num, combined_buf, read_mask);
    
    /* Retry once */
    if (ret)
        restore_sectors(block_num, combined_buf, read_mask);

    if (attempt != 0) {
        if (success)
            uart_printf("Successfully corrected block %d after %d attempts\n", block_num, attempt);
        else
            uart_printf("Could not correct block %d\n:(\n", block_num);
    }

    return !success;
}

/* Get the hardware ready, do some very basic sanity checks, and print storage into */
void bubble_storage_init()
{
    con_printf("Setting up function driver DMA\n");
    sequencer_init();

    detector_init();
    con_printf("Checking detector - ");

    if (detector_poll() != 0) {
        con_printf("\nDetector is HIGH?!\nCheck detector bias voltage\n");
        while(1);
    }

    con_printf("OK!\n");

    con_printf("Major/minor loop:%d/%d bubbles\n", MAJOR_LOOP_LEN, MINOR_LOOP_LEN);
    con_printf("Gen to Xfer gate: %d bubbles\n", GEN_TO_XFER_GATE);
    con_printf("Xfer to Annihilation gate: %d bubbls\n", XFER_GATE_TO_DET);
    con_printf("Ann gate to Detector: %d bubbles\n", DETECTOR_PRERUN_LEN);
    con_printf("Block redundancy: %d sectors\n", SECTOR_REDUNDANCY);
    con_printf("Capacity: %d blocks/%d bytes\n", NUM_BLOCKS, NUM_BLOCKS * BLOCK_LEN);

    uart_printf("Test sector: %d\n", TEST_SECTOR);
    uart_printf("Start sector: %d\n", START_SECTOR);
    uart_printf("Sector length: %d bytes\n", SECTOR_LEN);
    uart_printf("Block length: %d bytes\n", BLOCK_LEN);
    
    safe_drive();
}