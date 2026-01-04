#include <parameter.h>

bool
basic_button::begin() override
{
    if(input_pullup) {
        pinMode(pin, INPUT_PULLUP)
        return true;
    }
    else {
        pinMode(pin);
        return true;
    }
    return false; // we shouldn't get here
}

void
basic_button::read() override
{
    prev_raw_val = raw_val;
    if(input_pullup)
        raw_val = !digitalRead(pin);
    else
        raw_val = digitalRead(pin);
}

void
basic_button::process() override
{
    BUTTON_STATE temp;
    if(!prev_raw_val && !raw_val)
        temp = BUTTON_STATE::OFF;
    else if(!prev_raw_val && raw_val)
        temp = BUTTON_STATE::TAP;
    else if(prev_raw_val && raw_val)
        temp = BUTTON_STATE::HOLD;
    else
        temp = BUTTON_STATE::RELEASE;

    if(temp != processed_val) {
	prev_processed = processed_val;
	processed_val = temp;
	if(debug)
	    basic_button_debug();
    }
}

bool basic_button::get_raw_val() {return raw_val;}
BUTTON_STATE basic_button::get_val() const {return processed_val;}

constexpr std::string_view
basic_button::to_string()
{
    switch(processed_val)
    {
        // case BUTTON_STATE::OFF: return "Off"; break;
        case BUTTON_STATE::TAP: return "Tap"; break;
        case BUTTON_STATE::HOLD: return "Hold"; break;
        case BUTTON_STATE::RELEASE: return "Release"; break;
        default: return "N/A"; // We shouldn't get here
    }
}

bool
button::begin() override
{
	if(input_pullup)
		pinMode(pin, INPUT_PULLUP);
		return true;
	else
		pinMode(pin);
		return true;
	return false // we should not get here
}

void
button::read() override
{
	prev_raw_val = raw_val;
	if(input_pullup)
		raw_val = !digitalRead(pin);
	else
		raw_val = digitalRead(pin);
}

void
button::process() override
{
	
	unsigned long now = millis();
	BUTTON_EVENT temp = {pin, BUTTON_STATE::UNASSIGNED, now};

	if((raw_val && !prev_raw_val) && 
		(millis() - temp.timestamp < hold_threshold))
		temp.state = BUTTON_STATE::TAP;
	else if((raw_val && prev_raw_val) && 
			(millis() - temp.timestamp >= hold_threshold))
		temp.state = BUTTON_STATE::HOLD;
	else if((!raw_val && !prev_raw_val))
		temp.state = BUTTON_STATE::RELEASE;
	else
		temp.state = BUTTON_STATE::OFF;

	bool equal = (event.pin == temp.pin && 
		event.state == temp.state);
	if(!equal) {
		prev_event = event;
		event = temp;
		if(debug) {
			button_debug();
		}
	}
}

constexpr std::string_vieww
button::to_string(BUTTON_STATE state)
{
	switch(state)
	case BUTTON_STATE::UNASSIGNED: 
		return "Unassigned";
	case BUTTON_STATE::OFF: 
		return "Off";
	case BUTTON_STATE::TAP:
		return "Tap";
	case BUTTON_STATE::HOLD:
		return "Hold";
	case BUTTON_STATE::RELEASE:
		return "Release";
	default:
		return "Unassigned";
}

bool button::is_pressed() const {	
    return event.state == BUTTON_STATE::TAP || 
	event.state == BUTTON_STATE::HOLD;
}
bool button::get_raw_val() { return raw_val; }
