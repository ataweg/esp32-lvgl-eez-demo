echo "Remove ./main"
rm -rf ./main

echo "Copy ./examples/Common to ./main"
cp -r ./examples/Common ./main

echo "Copy ./examples/ESP32_2432s028 to ./main"
cp -rT ./examples/ESP32_2432s028 ./main

echo "Copy ./examples/ESP32_2432s028/sdkconfig to ./sdkconfig"
cp ./examples/ESP32_2432s028/sdkconfig ./sdkconfig
