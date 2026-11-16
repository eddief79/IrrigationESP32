import * as m from "zigbee-herdsman-converters/lib/modernExtend";

const endpoints = {
    zone_1: 1,
    zone_2: 2,
    zone_3: 3,
    zone_4: 4,
    zone_5: 5,
    zone_6: 6,
};

export default {
    zigbeeModel: ["IrrigationESP32"],
    model: "IrrigationESP32-6Z",
    vendor: "Open Irrigation",
    description: "Six-zone irrigation controller",
    extend: [
        m.deviceEndpoints({endpoints}),
        m.onOff({endpointNames: Object.keys(endpoints), powerOnBehavior: false}),
    ],
};