// ----------- BOARD TYPES ----------- //

#if   defined(ARDUINO_AVR_ADK)       
    #define BOARD "Mega Adk"
#elif defined(ARDUINO_AVR_BT)    // Bluetooth
    #define BOARD "Bt"
#elif defined(ARDUINO_AVR_DUEMILANOVE)       
    #define BOARD "Duemilanove"
#elif defined(ARDUINO_AVR_ESPLORA)       
    #define BOARD "Esplora"
#elif defined(ARDUINO_AVR_ETHERNET)       
    #define BOARD "Ethernet"
#elif defined(ARDUINO_AVR_FIO)       
    #define BOARD "Fio"
#elif defined(ARDUINO_AVR_GEMMA)
    #define BOARD "Gemma"
#elif defined(ARDUINO_AVR_LEONARDO)       
    #define BOARD "Leonardo"
#elif defined(ARDUINO_AVR_LILYPAD)
    #define BOARD "Lilypad"
#elif defined(ARDUINO_AVR_LILYPAD_USB)
    #define BOARD "Lilypad Usb"
#elif defined(ARDUINO_AVR_MEGA)       
    #define BOARD "Mega"
#elif defined(ARDUINO_AVR_MEGA2560)       
    #define BOARD "Mega 2560"
#elif defined(ARDUINO_AVR_MICRO)       
    #define BOARD "Micro"
#elif defined(ARDUINO_AVR_MINI)       
    #define BOARD "Mini"
#elif defined(ARDUINO_AVR_NANO)       
    #define BOARD "Nano"
#elif defined(ARDUINO_AVR_NG)       
    #define BOARD "NG"
#elif defined(ARDUINO_AVR_PRO)       
    #define BOARD "Pro"
#elif defined(ARDUINO_AVR_ROBOT_CONTROL)       
    #define BOARD "Robot Ctrl"
#elif defined(ARDUINO_AVR_ROBOT_MOTOR)       
    #define BOARD "Robot Motor"
#elif defined(ARDUINO_AVR_UNO)       
    #define BOARD "Uno"
#elif defined(ARDUINO_AVR_YUN)       
    #define BOARD "Yun"

// These boards must be installed separately:
#elif defined(ARDUINO_SAM_DUE)       
    #define BOARD "Due"
#elif defined(ARDUINO_SAMD_ZERO)       
    #define BOARD "Zero"
#elif defined(ARDUINO_ARC32_TOOLS)       
    #define BOARD "101"
#else
   #error "Unknown board"
#endif 
