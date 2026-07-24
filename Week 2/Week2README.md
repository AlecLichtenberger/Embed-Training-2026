# Week 2 - Signals and Comms

Starting from the fundamentals, signals can be broadly defined as some time-varying quantity that relays some information.

For our purposes, this quantity is most often voltage, but can occasionally be other measurable quantities like current. These signals can be broadly categorized as either analog or digital. 

## Analog Signals 

Analog signals are meant to be read in terms of some continuous interval, for example between 0V and 5V. While continuous, as you'd expect there's a limited amount of precision in the reading, which is usually +/- 0.1V or 0.01V. In the example below, we can see that we have some analog signal that has a voltage of 1.935V at t=4.34.

![Analog Signal Graph](Assets/week2%20Analog%20signal.png)

## Digital Signals

Digital signals are constricted to some set of discrete values, most commonly 0 and 1. This can be done through a variety of ways, such as setting some voltage to be a cut off (i.e. if V>0.5, count that as a 1, otherwise consider it a 0); however those details are beyond the scope of this training. An example of a digital signal is shown below. 

![Digital Signal Graph](Assets/week2%20digital%20signal.png)

Something you should be aware of when it comes to digital signals is that they rely on binary. Put simply, binary is a base-2 numbering system, as opposed to our traditional base 10 system. For example, consider the number 137. We can break down each digit as some multiple of a power of 10, as tabulated below. The only restriction is that the coefficients must be integers less than 10, otherwise it would overflow into the next power of 10.


| 10<sup>3</sup> | 10<sup>2</sup> | 10<sup>1</sup> | 10<sup>0</sup> |
|------|------|------|------|
| 0    | 1    | 3    | 7    |

(In other words, we can understand 137 as 100 + 30 + 7)

Binary is analogous to this system, except we're restricted only to coefficients of 1 and 0, and the base is 2, as you would expect. So we could represent the number 137 in binary as 

| 2<sup>7</sup> | 2<sup>6</sup> | 2<sup>5</sup> | 2<sup>4</sup> | 2<sup>3</sup> | 2<sup>2</sup> | 2<sup>1</sup> | 2<sup>0</sup> |
|------|------|------|------|------|------|------|------|
| 128  | 64   | 32   | 16   | 8    | 4    | 2    | 1    |
| 1    | 0    | 0    | 0    | 1    | 0    | 0    | 1    |

$ (1 * 128) + (0 * 64) + (0 * 32) + (0 * 16) + (1 * 8) + (0 * 4) + (0 * 2) + (1 * 1) = 137$

## Signal Processing and communications

While analog and digital are the two broad kinds of signals, when it comes to processing and interpreting signals in the real world, the field is very broad and complex, and beyond the scope of this training; although it should be noted that we do incorporate various filters for signal processing in embed. 

However, all you need to know for now, is that all signals have to eventually be processed and understood, and there are various protocols for that. For Embed, there are 2 main communication protocols you should be familiar with: CAN and I2C.

If you would like, we have a [video](https://youtu.be/Kz8soYV2pmk) briefly explaining these two protocols. At the end, it also explains SPI, which while useful, isn't something we expect you to currently know. Something that should be noted is that the video is from a few years ago, so the IMU pictured isn't actually the one we use now; however, all of the material is still accurate and applicable. 

For those of you who prefer to read, we have also written a short summary of the relevant communication protocols. Since I2C is a little simpler than CAN, we introduce it first, unlike the order shown in the video. 

Something you should be aware of is that these are **only summaries meant to introduce you to the fundamentals**, so there's a number of design nuances and limitations of these protocols that we don't cover here. **If you're curious about the details, ask your embed lead.**

## I2C: Inter-intergrated Circuit

I2C, which stands for inter-intergrated circuit, is a two-pin protocol between a controller (also known as a master device), and some number of peripheral devices (also known as slave devices) each with a unique address, as shown below. The purpose of the address is that so the controller can pick one particular device it wants to send or receive a message to at some time. This is done by sending a particular device's address to select it, and conversely, until a peripheral receives its exact address, it won't do anything except continue waiting. 

![I2C Organization diagram](Assets/week2%20i2c%20organization.png)


The two-pin denomination comes from the fact that the controller communicates to the peripherals via two pins: SCL and SDA. SCL, which is also denoted as SCK on some boards, stands for Serial Clock, and its purpose is to synchronize communication between the controller and peripheral. This is done by regularly flipping between high and low at some predetermined rate (typically 100khz). On the rising edge (when the clock goes from low to high), the SDA (Serial Data) pin is sampled, as shown below. 

![I2C Graph](Assets/week2%20I2C.jpg)

### A Quick Note on Addresses

Something we want to emphasize is that each I2C device comes with an intrinsic address set by the manufacturer, and there's a few details you should be aware of.

The first is that because each device has its address pre-set, you can't separately control duplicate peripherals with the same controller, so for example, you wouldn't hook up two of the same model IMU to your microcontroller.

The second thing you should be aware of, is that like we mentioned above, in order to do anything with a peripheral, you need to send out its address. However, the least significant bit (aka LSB, basically the last bit in the byte) of the address determines if you're accessing the peripheral in read-mode or write-mode. 

To illustrate this point with an example, when it comes to our IMU on our robots, the ISM330, the datasheet states that its address is  11010100. However, that's stated with the implict assumption that you know that the last 0 means that you're in write mode. So if you were to send 11010100 on the network, you would be accessing the IMU in write-mode. If you wanted to read from the IMU, you would have to send 11010101. 

For a more intuitive understanding of this, it may be helpful to mentally split this byte into sections, where the first 7 bits are the actual "address", and the last bit merely instructs the peripheral what's happening. This kind of division within a byte is actually fairly routine on peripherals, as each register (internal memory byte) on the device itself is divided into groups of bits that encode for something, as shown below. 

![IMU datasheet picture](Assets/datasheet%20picture.png)

## CAN: Controller Area Network 

While I2C is certainly useful, it is limited by its relatively low sampling rate, and it is susceptible to electromagnetic interference (EMI) and other sources of noise. As such, for certain applications, such as controlling motors, CAN is a much more suitable network than I2C.

CAN and I2C are similar in terms of how they handle addresses (i.e. a peripheral only responds/does something when it receives its address), but CAN differs from I2C in one key way relevant to us: differential signalling.

Differential signaling is a fancy term that means the signal is read from the voltage difference between two wires (CAN_H and CAN_L) instead of having a single SDA wire. These two wires are meant to be coiled together, so that any EMI should impact them equally, thus preserving their difference. 

This resistance to EMI makes CAN well suited for communication between our micro-controller and our motors, since the motors themselves generate significant EMI, and CAN has to be responsible for all of our motors, from the wheels to the pitch and yaw of the turret, so the robustness of CAN ensures consistent communication.  

For completeness, here's a simple wiring diagram for CAN. Be aware that red is for CAN high, and black is for CAN low. 

![CAN Wiring Diagram](Assets/week2%20CAN%20wiring%20diagram.png)


# Assignment: Implementing BNO055 IMU

Your assignment for this week is implementing a simple driver for the BNO055 IMU. This driver should be able to initalize the BNO055, and grab XYZ acceleration and gyro data. Once you're able to do that, make sure you set the BNO055 to 9DOF mode, and then start grabbing the pitch, roll, and yaw from it as well. 

**Once you're done, copy your assignment code into the BNO055 files in the core.** They're located in core/util/peripherals/imu

To help you, here's a [link](https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bno055-ds000.pdf) to the datasheet (HINT: Start by looking for the relevant registers, and also find the constants to convert accel and gyro register readings into real units), and we'll explain some of the relevant i2c functions below. 

[TODO: Add a section on device tree when we get that up and running]

The i2c write function (i2c_write_dt) and read function (i2c_read_dt) take in a pointer to the device, an array you want to write, and the # of bytes you're writing out. So for example, let's say 

[TODO: ADD EXAMPLE]

Note that in order to properly read a register, you first have to write out that register, so you may want to use the combined write/read function for that, which is shown below 

[TODO: ADD EXAMPLE]

If there's anything you're confused about, please ping us before you ask Claude or your LLM of choice. We were all recruits once, are happy to help in any way we can. 