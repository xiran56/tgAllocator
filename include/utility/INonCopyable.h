#pragma once

/* Интерфейс некопируемого класса */

class INonCopyable {
    public:
        INonCopyable(const INonCopyable&) = delete;

        INonCopyable operator =(const INonCopyable&) = delete;

    protected:
        INonCopyable() {  }

        ~INonCopyable() {  }
};
