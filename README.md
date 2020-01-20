# Jeep_Dev
The idea is to create a touchscreen interface for controlling (simple on|off or multimode) lights, horns (any relay driven) and other API driven light sources
WLED will be the primary integration focus initially for API interfaces

## Jeep_Simple

Contains the initial light class with two inheriting classes FogMM (a [6 mode light](https://www.amazon.com/gp/product/B07XH1QWJ6/ref=ppx_yo_dt_b_asin_title_o07_s01?ie=UTF8&psc=1 "6 mode light")) and Spots (simple On|Off) controling 4 [30V 10A relay boards](https://www.amazon.com/gp/product/B07PNB86R7/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1 "30V 10A relay boards") that have a logic level of 3.3V. In Nextion folder there is a Visio Doc that was used to create the user interface, the TFT and HMI files for [Nextion 4.3 basic display](https://www.itead.cc/nextion-nx4827t043.html "Nextion 4.3 basic display") and an images folder used for the GUI. 

## WishList
&curren; On|Off for MM lights
&curren; Flash mode for simple On\Off lights
&curren; Speed control for simple flash mode
&curren; New inherited class for API diven lights