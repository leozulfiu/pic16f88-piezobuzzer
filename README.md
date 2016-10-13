# pic16f88 piezobuzzer

With this code you can play any music you like on a [pic16f88](http://www.microchip.com/wwwproducts/en/PIC16F88) connected to a [piezo buzzer](http://shallowsky.com/arduino/class/buzzer.jpg). The sound is generated via the Capture, Compare, PWM (CCP) module, which generates PWM signals in the corresponding frequencies.

## Usage

Clone the code and build the needed hex file with the XC8 compiler. Use PICkit2 to program the microcontroller. In the main file, you can change the song which will be played. Change these two arrays as you wish. The beats array describes the respective note length in the typical musical notation (half note, quarter note, ...).

```
short melody[] = {g, g, a, g, C, b, g, g, a, g, D, C, g, g, G, E, C,
                  b, a, F, F, E, C, D, C};
char beats[] =   {8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4, 2, 8, 8, 4, 4, 4,
                  4, 4, 8, 8, 4, 4, 4, 2};
```

The shown arrays represent for example the happy birthday melody.

## Scheme

The PWM signal will be generated at pin RB3, which you have to configure in the configuration bits. See conf.h at CCPMX bit.

<img src="https://github.com/leozulfiu/pic16f88-piezobuzzer/blob/master/screenshots/scheme.png" width="300">

## License

MIT
