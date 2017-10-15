#ifndef LOGIC_DATA
#define LOGIC_DATA

class LogicData {
    public:
        LogicData(unsigned char pin);
        unsigned char process();
    private:
        unsigned char _pin;
        unsigned char _currentPos;
        unsigned char _heightArray[8];
        unsigned char _receiveArray[23];
        unsigned long _lastTime;
        bool checkTime();        
        bool checkParity(unsigned char currentBit);
        unsigned char calculateHeight();
        bool checkArray();
};

#endif
