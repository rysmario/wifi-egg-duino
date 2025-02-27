#ifndef PRINTER_H
#define PRINTER_H

#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Preferences.h>
#include <FS.h>

typedef std::function<void()> PrinterHandler;

#define PIN_ROT_DIR 33
#define PIN_ROT_STEP 32
#define PIN_ROT_RES 25

#define PIN_PEN_DIR 27
#define PIN_PEN_STEP 26
#define PIN_PEN_RES 25

#define PIN_SERVO 14
#define SERVO_CHA 2
#define SERVO_MIN (65536 / 20)
#define SERVO_MAX (2 * SERVO_MIN)

#define VSPI_SS 4
#define VSPI_SCLK 18
#define VSPI_MISO 19
#define VSPI_MOSI 23

struct MotionParameters
{
    uint8_t penUpPercent, penDownPercent;
    uint16_t drawingSpeed, travelSpeed;
    uint16_t penMoveDelay;
    uint16_t stepsPerRotation;
    bool reverseRotation, reversePen;
};

class Printer
{
public:
    Printer();
    void begin();

    void penUp();
    void penDown();
    void enableMotors();
    void disableMotors();
    void getParameters(MotionParameters &params);
    void setParameters(const MotionParameters &params);

    void print(File file);
    void stop();
    void pause();
    void continuePrint();

    bool isPaused() { return waiting; }
    String getWaitingFor() { return waitingFor; }
    bool isPrinting() { return printTaskHandle ? true : false; }
    const ulong getPrintedLines() { return printedLines; }
    const char *printingFileName() { return printing->name(); }

    void onProgressChanged(PrinterHandler handler) { onProgress = handler; }
    void onStatusChanged(PrinterHandler handler) { onStatus = handler; }

    void printTask();

private:
    void applyParameters();
    void moveTo(long x, long y);

    bool waiting;
    File *printing;

    ulong printedLines;
    PrinterHandler onProgress, onStatus;
    String waitingFor;
    AccelStepper mRotation, mPen;
    MultiStepper multiStepper;

    int32_t posX, posY;
    uint16_t penUpValue, penDownValue,penCurrValue;
    bool _isPenUp, _isMoving;
    Preferences preferences;
    MotionParameters parameters;
    TaskHandle_t printTaskHandle = NULL;
};

#endif
