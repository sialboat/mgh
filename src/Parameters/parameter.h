#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#pragma once

/*
    ========================================
    parameter.h parameter objects
    ========================================

    Abstractions for creating objects to easily read, store, and process information from
    sensors. An optional debug() method exists to allow for serial printing of some sort.
*/

/*
    ========================================
    base parameter interface
    ========================================
*/
class base_param
{
    virtual bool begin() = 0;   // initialize parameter
    virtual void read() = 0;    // read raw values 
    virtual void debug(const char* msg) = 0;   // enable serial printing?
    virtual void process() = 0; // basic processing for the parameter like smoothing (if applicable)
};

/*
    ========================================
    Base parameter class
    ========================================
*/
template <typename N>
class param : public base_param
{
    // Using this to allow users to use custom callbacks
    using debug_callback = void (*)(const char* msg);

    public:
    param(Adafruit_USBD_CDC& s) : serial(s), custom_debug(nullptr) {}
    ~param() = default;

    virtual bool begin() override;
    virtual void read() override;
    virtual void process() override;

    virtual void debug(const char* msg) override {
        if(debug_routine)
            debug_routine(msg);
        else {
            serial.print("DEBUG: ");
            serial.println(msg);
        }
    }

    void set_debug_callback(debug_callback cb) {
        debug_routine = cb;
    }

    protected:
    debug_callback debug_routine;
    Adafruit_USBD_CDC& serial;
};

class basic_button: public param<bool>
{
    enum class BUTTON_STATE : uint8_t {
        OFF = 0, TAP = 1, HOLD = 2, RELEASE = 4
    };

    public:
    basic_button(int p, bool i, bool o, Adafruit_USBD_CDC& s) : pin(p), 
                input_pullup(i), param(s), 
		processed_val(BUTTON_STATE::OFF),
                prev_processed(BUTTON_STATE::OFF), debug(false) {}

    public:
    bool begin() override;
    void read() override;
    void process() override;

    void basic_button_debug() {
        serial.print("Basic Button: ");
        serial.print("(input pullup: ");
        serial.print(input_pullup)
        serial.print("), ");
        serial.print(pin);
        serial.print(", ");
        serial.println(to_string());
    }

    void basic_button_debug_complex() {
        serial.print("Basic Button: ");
        serial.print("(input pullup: ");
        serial.print(input_pullup)
        serial.print("), ");
        serial.print(pin);
        serial.print(", ");
        serial.print(raw_val);
        serial.print(", ");
        serial.print(prev_raw_val);
        serial.print(", ");
        serial.println(to_string());
    }

    bool get_raw_val();
    BUTTON_STATE get_val() const;
    constexpr std::string_view to_string();

    private:
    int pin;
    bool input_pullup;
    BUTTON_STATE processed_val;
    BUTTON_STATE prev_processed;
    bool raw_val;
    bool prev_raw_val;
    bool debug;
};

/*
    ========================================
    Button Parameter Class
    ========================================

    Button class inspired by the OneButton class by Matthias Hertel,
    implements multi-state button 
*/
class button : public param<bool>
{
    enum class BUTTON_STATE {
        UNASSIGNED = -1, OFF = 0, TAP = 1, HOLD = 2, RELEASE = 3
    };

    typedef struct BUTTON_EVENT {
	int button_pin;
	BUTTON_STATE state;
	unsigned long timestamp;
    } BUTTON_EVENT;

public:
	button(int p, bool in_p, bool o, Adafruit_USBD_CDC& c) : pin(p)
		, input_pullup(in_p),
		param(c), raw_val(false), prev_raw_val(false),
		event(nullptr), prev_event(nullptr), 
		hold_threshold(500), {}
public:
	bool is_pressed() const;
	bool get_raw_val();
	constexpr std::string_view to_string();

	void button_debug() {
	    serial.print("Button: ");
	    serial.print("(input pullup: ");
	    serial.print(input_pullup);
	    serial.print(", ");
	    serial.print(to_string(event.state));
	}

	bool begin() override;
	void read() override;
	void process() override;
	
private:
	int pin;
	unsigned long hold_threshold;
	BUTTON_EVENT event;
	BUTTON_EVENT prev_event;
	bool input_pullup;
	bool raw_val;
	bool prev_raw_val;
};

/*
    ========================================
    Potentiometer Parameter Class
    ========================================
    
    Potentiometer parameter class that abstracts the analog read
    and data collection from potentiometer sensors. Utilizes the
    ADC class that natively comes with the Teensy to create fast
    and asynchronous reads for CPU efficient reads.
*/
class potentiometer : public param<int>
{
public:

private:

};
/*
    ========================================
    Switch Parameter Class
    ========================================
*/

/*
    ========================================
    Joystick Parameter Class
    ========================================
*/

/*
    ========================================
    Gyroscope Parameter Class
    ========================================
*/
