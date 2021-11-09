# Introduction To Robotics

Come see my robots! - Robotics projects completed during the Introduction To Robotics course @ CS, UNIBUC

<div align="center"><img src="https://imgs.xkcd.com/comics/the_three_laws_of_robotics.png"> </div>


If you've yet to read Asimov's Robots series, I highly recommend it. 

<hr>

#### Homework #0
Consisted of installing Arduino IDE and setting up this very repository. Hi!

#### Homework #1

##### Task Requirements

Use 3 potentiometers to digitally control a RGB led.

##### Picture
![tema1](assets/tema1.jpeg)
##### Video
<a href="https://www.youtube.com/watch?v=cUwO6YBgpMw"><img src="https://img.youtube.com/vi/cUwO6YBgpMw/0.jpg" alt="video hw 1"></a>

#### Homework #2

##### Task Requirements

Create a traffic light simulator with both car and pedestrian lights, as well as a buzzer. On a button press, the system will go through the following states:

 - State 1 (default, reinstated after state 4 ends): green light for cars,red  light  for  people,  no  sounds.   Duration:  indefinite,  changed  bypressing the button.
 - State 2 (initiated by counting down 10 seconds after a button press):the  light  should  be  yellow  for  cars,  red  for  people  and  no  sounds.Duration:  3 seconds.
 - State 3 (iniated after state 2 ends): red for cars, green for people anda beeping sound from the buzzer at a constant interval.  Duration:10 seconds.
 - State 4 (initiated after state 3 ends): red for cars,blinking green for people and a beeping sound from the buzzer, at a constant interval,  faster than the beeping in state 3. This state should last 5 seconds.

Subsequent button presses should not reset the system until after it is idling again.

Write the code without using the `delay()` function!

##### Picture
![tema2](assets/tema2.jpeg)
##### Video
<a href="https://youtu.be/LNvTyDPz8G8"><img src="https://img.youtube.com/vi/LNvTyDPz8G8/0.jpg" alt="video hw 2"></a>

#### Homework #3

##### Task Requirements

Create an Electromagnetic Field Detector (or ghost detector if you may) that shows data on a 7-segment display, as well as buzzes, when approaching electromagnetic fields.

##### Picture
![tema2](assets/3.jpg)
##### Video
<a href="https://youtu.be/XFmxfKWtlRE"><img src="https://img.youtube.com/vi/XFmxfKWtlRE/0.jpg" alt="video hw 3"></a>
