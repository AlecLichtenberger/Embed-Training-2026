# Week 2 - Signals and Comms

Starting from the fundamentals, signals can be broadly defined as some time-varying quantity that relays some information.

For our purposes, this quantity is most often voltage, but can occasionally be other measurable quantities like current. These signals can be broadly categorized as either analog or digital. 

## Analog Signals 

Analog signals are meant to be read in terms of some continuous interval, for example between 0V and 5V. While continuous, as you'd expect there's a limited amount of precision in the reading, which is usually +/- 0.1V or 0.01V. In the example below, we can see that we have some analog signal that has a voltage of 1.935V at t=4.34.

![Analog Signal Graph](Assets/week1%20Analog%20signal.png)

## Digital Signals

Digital signals are constricted to some set of discrete values, most commonly 0 and 1. This can be done through a variety of ways, such as setting some voltage to be a cut off (i.e. if V>0.5, count that as a 1, otherwise consider it a 0); however those details are beyond the scope of this training. An example of a digital signal is shown below. 

![Digital Signal Graph](Assets/week1%20digital%20signal.png)

Something you should be aware of when it comes to digital signals is that they rely on binary. Put simply, binary is a base-2 numbering system, as opposed to our tradational base 10 system. For example, consider the number 137. We can break down each digit as some multiple of a power of 10, as tabulated below. The only restriction is that the coefficients must be integers less than 10, otherwise it would overflow into the next power of 10.


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

However, all you need to know for now, is that all signals have to eventually be processed and understood, and there are various protocols for that. For Embed, there are 2 main communication protocols you should be familiar with: I2C and CAN. 

## I2C: Inter-intergrated Circuit

## Controller Area Network (CAN)

# Assignment: Implementing BNO055 IMU