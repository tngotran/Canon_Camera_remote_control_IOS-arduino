# Canon_Camera_remote_control_IOS-arduino
Remote control for Canon 50D camera

1st part: arduino, that physically control the camera
2nd part: ios app that install app on your iphone and communicate the 1st part

the circuit to make a switch with BJT
 https://tngotran.wordpress.com/2017/01/23/nail-fold-capillaroscopy-device/
 
 first turn on HM10/bluetooth/receiver device.
 Dont try to pair with device from the android/setting
 
 open terminal app
 switch to device tab, search under LE device or try classic
 then select the device
 then go back to main page, press on connect to device button
 then voila :) have a nice day
 
 ===============================
 below are commands to set on your android/ios app. Available on the apps store, some predifine app allow you to modify what to send
  
focus = a   //for focus, send charater 'a'
shoot = b   //for shoot a photo, send a character 'b'
cancel = c  //after focus, you can cancel, dont focus any more by sending a character 'c'

3sOn = d    //shoot after 3 seconds
3sOf = e    //turn off shoot after 3 seconds
5sOn = f
5sOf = g

3tOn = h shoot 3 times
3tOf = i
5tOn = j
5tOf = k

sound on = l
sound off = m
sound notify disconection = n
sound nofity connected = o
sound for shooting = p

================================
 
 Useful AT commands
 both serial baur rate are 9600
 
 
 AT enter -> return OK
 AT+HELP -> print out all available commands. Because of many clone versions on the market, then not surprise if your version only support limited number of commands
 AT+POWE3 -> set the power to highest value, to transmit from a distance.
 AT+NAME
 blablabla
 check more on the datasheet DS_IM130614.pdf
 
 ===================
 
