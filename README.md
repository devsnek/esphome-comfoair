# ComfoAir

Port of the ComfoAir protocol to ESPHome.io firmware, which is supported by external_components.

Add the following definition of `external_components` to your yaml configuration:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/wichers/esphome-comfoair
    components: [comfoair]
```

And configuration for the component:

```yaml
uart:
  id: uart_bus
  tx_pin: GPIO19 # Adjust for your setup
  rx_pin: GPIO22 # Adjust for your setup
  baud_rate: 9600
  parity: NONE
  stop_bits: 1
  data_bits: 8

comfoair:
  name: "WHR 930" # Adjust for your setup
  uart_id: uart_bus
```

For visualization:
Checkout https://github.com/wichers/lovelace-comfoair and follow the instructions.

## Compatibility

This is a list of models that are known to work, feel free to add yours if you tested it successfully:

* Zehnder WHR 930
* Zehnder WHR 950
* Zehnder ComfoAir 160
* Zehnder ComfoAir 200
* Zehnder ComfoAir 500
* Zehnder Comfoair 550
* Zehnder ComfoD 300
* Zehnder ComfoD 350
* Zehnder ComfoD 450
* Zehnder ComfoD 550

## Incompatible

* Zehnder ComfoAir Q350 (CAN)
* Zehnder ComfoAir Q450 (CAN)
* Zehnder ComfoAir Q600 (CAN)

## Support my work

This repository is a fork, support the original author:
[!["Buy Me A Coffee"](https://www.buymeacoffee.com/assets/img/custom_images/orange_img.png)](https://www.buymeacoffee.com/wichers)
