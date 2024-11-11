// #include "CircularDisplay.h"

// CircularDisplay::CircularDisplay(/* args */) {}

// CircularDisplay::~CircularDisplay() {}

// void CircularDisplay::setup() {
//   pinMode(RES, OUTPUT);
//   pinMode(DDC, OUTPUT);
//   pinMode(DCS, OUTPUT);
//   SPI.begin();

//   GC9A01_init();
//   struct GC9A01_frame frame = {{0, 0}, {239, 239}};
//   GC9A01_set_frame(frame);
// }

// void CircularDisplay::loop() {
//   uint8_t color[3];

//   // Triangle
//   color[0] = 0xFF;
//   color[1] = 0xFF;
//   for (int x = 0; x < 240; x++) {
//     for (int y = 0; y < 240; y++) {
//       if (x < y) {
//         color[2] = 0xFF;
//       } else {
//         color[2] = 0x00;
//       }
//       if (x == 0 && y == 0) {
//         GC9A01_write(color, sizeof(color));
//       } else {
//         GC9A01_write_continue(color, sizeof(color));
//       }
//     }
//   }

//   delay(1000);

//   // Rainbow
//   float frequency = 0.026;
//   for (int x = 0; x < 240; x++) {
//     color[0] = sin(frequency * x + 0) * 127 + 128;
//     color[1] = sin(frequency * x + 2) * 127 + 128;
//     color[2] = sin(frequency * x + 4) * 127 + 128;
//     for (int y = 0; y < 240; y++) {
//       if (x == 0 && y == 0) {
//         GC9A01_write(color, sizeof(color));
//       } else {
//         GC9A01_write_continue(color, sizeof(color));
//       }
//     }
//   }

//   delay(1000);

//   // Checkerboard
//   for (int x = 0; x < 240; x++) {
//     for (int y = 0; y < 240; y++) {
//       if ((x / 10) % 2 == (y / 10) % 2) {
//         color[0] = 0xFF;
//         color[1] = 0xFF;
//         color[2] = 0xFF;
//       } else {
//         color[0] = 0x00;
//         color[1] = 0x00;
//         color[2] = 0x00;
//       }
//       if (x == 0 && y == 0) {
//         GC9A01_write(color, sizeof(color));
//       } else {
//         GC9A01_write_continue(color, sizeof(color));
//       }
//     }
//   }

//   delay(1000);

//   // Swiss flag
//   color[0] = 0xFF;
//   for (int x = 0; x < 240; x++) {
//     for (int y = 0; y < 240; y++) {
//       if ((x >= 1 * 48 && x < 4 * 48 && y >= 2 * 48 && y < 3 * 48) ||
//           (x >= 2 * 48 && x < 3 * 48 && y >= 1 * 48 && y < 4 * 48)) {
//         color[1] = 0xFF;
//         color[2] = 0xFF;
//       } else {
//         color[1] = 0x00;
//         color[2] = 0x00;
//       }
//       if (x == 0 && y == 0) {
//         GC9A01_write(color, sizeof(color));
//       } else {
//         GC9A01_write_continue(color, sizeof(color));
//       }
//     }
//   }

//   delay(1000);
// }