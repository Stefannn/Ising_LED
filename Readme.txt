Simulate Ising Model and Visualize it on a LED panel hooked to a rasperry pi

Code for LED-access (in subfolder LED) from:
https://github.com/hzeller/rpi-rgb-led-matrix


Can be extended by providing own ModelPolicy / ViewPolicy classes (Potts, Kawasaki...), the requirements are stated in the Simulation class.



To Run:
[Run on a raspberry pi with a 16x32 LED-matrix; tested on a product ordered from adafruit]

$make
$sudo ./isingLED [nSeconds to simulate] [inverse temperature]

(sudo required to access /dev/mem)


