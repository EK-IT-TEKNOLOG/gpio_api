#  gpio_api

Put this app in the `ArduinoApps` folder and start it with:
```bash
arduino-app-cli app start user:gpio_api
```

Stop using:
```
arduino-app-cli app stop user:gpio_api
```

Set as default (so it starts on boot):
```
arduino-app-cli properties set default user:gpio_api
```

# More usage

Clone the repo into the right folder:
   
```
cd ArduinoApps
git clone https://github.com/EK-IT-TEKNOLOG/gpio_api.git
```

Start the app using the commands above

Clone the python module to use:

```
cd
git clone https://github.com/EK-IT-TEKNOLOG/uno_machine.git
cd uno_machine
```

Now write your code. See the `test.py` as starting point.
