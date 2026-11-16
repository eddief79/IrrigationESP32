# Zigbee2MQTT setup

The ESP32-C6 joins an existing Zigbee network as a mains-powered router. It does not form a network or replace the Zigbee coordinator.

1. Copy `external_converters/irrigationesp32.mjs` into the Zigbee2MQTT `external_converters` directory, beside its `configuration.yaml`.
2. On Zigbee2MQTT 2.11.0 and newer, enable `advanced.enable_external_js` in its configuration and restart Zigbee2MQTT.
3. Enable joining in Zigbee2MQTT, then power up the ESP32-C6. A factory-new device starts network steering on boot.
4. After the interview, Zigbee2MQTT should expose `zone_1` through `zone_6` as switches. With Home Assistant discovery enabled in Zigbee2MQTT, the switches should be discovered by Home Assistant.

The implementation uses the ESP-IDF Zigbee component (`espressif/esp-zigbee-lib` 2.1.x). ESP-IDF 5.5.6 is the SDK's recommended version. The project currently has an ESP-IDF 6.1.0-generated sdkconfig; build compatibility with that version has not been verified.

## Current limitation

The controller allows one watering zone at a time. Requesting another zone sends a stop request for the current zone before starting the new one. The local zone state is serialized by the irrigation task; endpoint-state reporting during a zone-to-zone change should still be verified on hardware.