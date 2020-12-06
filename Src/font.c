#include "font.h"

const static uint8_t bfont_data[]={
    0,0,0,0,0,            // Space
    0,0,95,0,0,           // !
    0,0,3,0,0,            // "
    20,127,20,127,20,     // #
    36,42,127,42,18,      // $
    35,19,8,100,98,       // %
    0x07,0x05,0x07,0x00,0x00,       // Degree Symbol = &
    0,5,3,0,0,            // '
    0,62,65,0,0,          // (
    0,65,62,0,0,          // )
    20,8,62,8,20,         // *
    8,8,62,8,8,           // +
    0,80,48,0,0,          // ,
    8,8,8,8,8,            // -
    0,96,96,0,0,          // .
    32,16,8,4,2,          // /
    62,81,73,69,62,       //0
    0,66,127,64,0,        //1
    66,97,81,73,70,       //2
    33,65,69,75,49,       //3
    24,20,18,127,16,      //4
    39,69,69,69,57,       //5
    60,74,73,73,48,       //6
    1,1,121,5,3,          //7
    54,73,73,73,54,       //8
    6,73,73,41,30,        //9
    0,54,54,0,0,          //:
    0,86,54,0,0,          //;
    8,20,34,65,0,         //<
    20,20,20,20,20,       //=
    65,34,20,8,0,         //>
    2,1,81,9,6,           //?
    50,73,121,65,62,      //@
    126,9,9,9,126,        //A
    127,73,73,73,54,      //B
    62,65,65,65,34,       //C
    127,65,65,65,62,      //D
    127,73,73,73,65,      //E
    127,9,9,9,1,          //F
    62,65,73,73,122,      //G
    127,8,8,8,127,        //H
    0, 65,127,65,0,       //I
    32,64,65,63,1,        //J
    127,8,20,34,65,       //K
    127,64,64,64,64,      //L
    127,2,12,2,127,       //M
    127,4,8,16,127,       //N
    62,65,65,65,62,       //O
    127,9,9,9,6,          //P
    62,65,81,33,94,       //Q
    127,9,25,41,70,       //R
    70,73,73,73,49,       //S
    1,1,127,1,1,          //T
    63,64,64,64,63,       //U
    31,32,64,32,31,       //V
    63,64,48,64,63,       //W
    99,20,8,20,99,        //X
    3,4,120,4,3,          //Y
    97,81,73,69,67,       //Z
    0,127,65,0,0,         //[
    16,38,32,38,16,       //:)
    0,65,127,0,0,         //]
    4,2,1,2,4,            //^
    64,64,64,64,64,       //_
    0,1,2,4,0,            //`
    32,84,84,84,120,      //a
    127,72,68,68,56,      //b
    56,68,68,68,32,       //c
    56,68,68,72,127,      //d
    56,84,84,84,24,       //e
    8,126,9,1,2,          //f
    12,82,82,82,60,       // g
    127,8,4,4,120,        //h
    0,68,125,64,0,        //i
    32,64,68,61,0,        //j
    127,16,40,68,0,       //k
    0,65,127,64,0,        //l
    124,4,24,4,120,       //m
    124,8,4,4,120,        //n
    56,68,68,68,56,       //o
    124,20,20,20,8,       //p
    8,20,20,24,124,       //q
    124,8,4,4,8,          //r
    72,84,84,84,32,       //s
    4,63,68,64,32,        //t
    60,64,64,32,124,      //u
    28,32,64,32,28,       //v
    60,64,48,64,60,       //w
    68,40,16,40,68,       //x
    12,80,80,80,60,       //y
    68,100,84,76,68,      //z
    0,8,54,65,0,          //}
    0,0,127,0,0,          //|
    0,65,54,8,0,          //{
    8,8,42,28,8,          //->
    8,28,42,8,8           //<-
};

const uint8_t rfont_data[] =
{
    0x00, 0x00, 0x00,   // Space
    0x00, 0x17, 0x00,   // !
    0x07, 0x00, 0x07,   // "
    0x1F, 0x0A, 0x1F,   //  #   // Number Entry Mode
    0x00, 0x1F, 0x00,   //  $   // Down Arrow
    0x00, 0x00, 0x00,   //  %
    0x07, 0x05, 0x07,   //  &   // Degree Symbol
    0x00, 0x03, 0x00,   // '
    0x00, 0x0E, 0x11,   // (
    0x11, 0x0E, 0x00,   // )
    0x08, 0x1C, 0x0F,   // *    // Done Entry
    0x04, 0x06, 0x04,   // +
    0x10, 0x0C, 0x00,   // ,
    0x04, 0x04, 0x04,   // -
    0x00, 0x10, 0x00,   // .
    0x18, 0x04, 0x03,   // /

    0x0E, 0x11, 0x0E,   // 0
    0x12, 0x1F, 0x10,   // 1
    0x19, 0x15, 0x12,   // 2
    0x11, 0x15, 0x0A,   // 3
    0x07, 0x04, 0x1E,   // 4
    0x17, 0x15, 0x09,   // 5
    0x1E, 0x15, 0x1D,   // 6
    0x19, 0x05, 0x03,   // 7
    0x1F, 0x15, 0x1F,   // 8
    0x17, 0x15, 0x0F,   // 9
    0x00, 0x0A, 0x00,   // :
    0x00, 0x00, 0x00,   // ;
    0x00, 0x00, 0x00,   // <
    0x00, 0x00, 0x00,   // =
    0x00, 0x00, 0x00,   // >
    0x00, 0x00, 0x00,   // ?
    0x00, 0x00, 0x00,   // @


    0x1E, 0x05, 0x1E,   //A
    0x1F, 0x15, 0x0A,   //B
    0x0E, 0x11, 0x11,   //C
    0x1F, 0x11, 0x0E,   //D
    0x1F, 0x15, 0x11,   //E
    0x1F, 0x05, 0x01,   //F
    0x0E, 0x11, 0x1D,   //G
    0x1F, 0x04, 0x1F,   //H
    0x11, 0x1F, 0x11,   //I
    0x18, 0x10, 0x1F,   //J
    0x1F, 0x04, 0x1B,   //K
    0x1F, 0x10, 0x10,   //L
    0x1F, 0x06, 0x1F,   //M
    0x1F, 0x01, 0x1E,   //N
    0x1F, 0x11, 0x1F,   //O
    0x1F, 0x05, 0x02,   //P
    0x0F, 0x19, 0x1F,   //Q
    0x1F, 0x05, 0x1A,   //R
    0x12, 0x15, 0x09,   //S
    0x01, 0x1F, 0x01,   //T
    0x1F, 0x10, 0x1F,   //U
    0x07, 0x18, 0x07,   //V
    0x1F, 0x08, 0x1F,   //W
    0x1B, 0x04, 0x1B,   //X
    0x03, 0x1C, 0x03,   //Y
    0x19, 0x15, 0x13,   //Z

    0x02, 0x1F, 0x02,   // [    // Shift
    0x00, 0x00, 0x00,   // :)
    0x00, 0x00, 0x00,   // ]
    0x00, 0x00, 0x00,   // ^
    0x10, 0x10, 0x10,   // _
    0x00, 0x00, 0x00,   // `


    /* PUT IN LOWERCASE HERE */
    0x0C, 0x12, 0x1E,   //a
    0x1F, 0x12, 0x0C,   //b
    0x0C, 0x12, 0x12,   //c
    0x0C, 0x12, 0x1F,   //d
    0x0C, 0x1A, 0x14,   //e
    0x1E, 0x05, 0x01,   //f

    0x12, 0x15, 0x0F,   //g
    0x1F, 0x02, 0x1C,   //h
    0x00, 0x1D, 0x00,   //i
    0x08, 0x10, 0x0D,   //j
    0x1F, 0x08, 0x14,   //k

    0x11, 0x1F, 0x10,   //l
    0x1E, 0x04, 0x1E,   //m
    0x1E, 0x02, 0x1C,   //n
    0x0C, 0x12, 0x0C,   //o
    0x1E, 0x0A, 0x04,   //p

    0x04, 0x0A, 0x1E,   //Q
    0x1E, 0x04, 0x02,   //R
    0x16, 0x1A, 0x00,   //S
    0x0F, 0x12, 0x10,   //T
    0x1E, 0x10, 0x1E,   //U
    0x06, 0x18, 0x06,   //V

    0x1E, 0x08, 0x1E,   //W
    0x12, 0x0C, 0x12,   //X
    0x16, 0x08, 0x06,   //Y

   // 0x19, 0x15, 0x13,   //Z
    0x00, 0x00, 0x00,   // {
    0x00, 0x00, 0x00,   // |
    0x00, 0x00, 0x00,   // }
    0x00, 0x00, 0x00,   // ->
    0x00, 0x00, 0x00    // <-
};

const struct font font_5x7 = {
    .width = 5,
    .height = 7,
    .data = bfont_data,
};

const struct font font_3x5 = {
    .width = 3,
    .height = 5,
    .data = rfont_data,
};