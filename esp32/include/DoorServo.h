// since ESP32 does not have the Arduino built-in library, this is a fork found on GitHub
#include <ESP32Servo.h>

class DoorServo
{
public:
    Servo servo; // servo method from ESP32Servo
    bool lock_status = true;
    const long pin_servo_control = GPIO_NUM_18; // GPIO to servo PWM

    void setup()
    {
        // Allow allocation of all timers
        ESP32PWM::allocateTimer(0);
        ESP32PWM::allocateTimer(1);
        ESP32PWM::allocateTimer(2);
        ESP32PWM::allocateTimer(3);

        // standard 50hz, modify for other servos
        servo.setPeriodHertz(50);

        // for the FS5103R the pulse width range is between 700 and 2300
        // use other values if you are using another servo
        servo.attach(pin_servo_control, 700, 2300);
    }

    /**
     * Write servo position
     * 
     * @param pos integer "degree"/pos to send to servo (degree for non-360 deg servos)
     * @return void
     */
    void turn_lock(int pos)
    {
        setup();
        servo.write(pos);
        delay(5000);
        servo.detach();
    }

    void lock()
    {
        turn_lock(180);
    }

    void unlock()
    {
        turn_lock(0);
    }

    /**
     * Set param value as lock_status and turn servo based on state
     * 
     * @param new_val Boolean new value for lock. true: locked, false: unlocked
     * @return void
     */
    void sync(bool new_val)
    {
        // only speak with servo if it has changed
        if (new_val != lock_status)
        {
            lock_status = new_val;
            if (lock_status)
            {
                Serial.println("Locking door");
                lock();
            }
            else
            {
                Serial.println("Unlocking door");
                unlock();
            }
        }
    }
};
