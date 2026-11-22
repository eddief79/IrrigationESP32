import {Zcl} from "zigbee-herdsman";
import * as m from "zigbee-herdsman-converters/lib/modernExtend";

const endpoints = {
    zone_1: 1,
    zone_2: 2,
    zone_3: 3,
    zone_4: 4,
    zone_5: 5,
    zone_6: 6,
};

const irrigationErrors = {
    no_error: 0,
    water_flow_while_idle: 1,
    stop_requested_while_idle: 2,
    no_water_flow: 3,
    maximum_duration_exceeded: 4,
    maximum_volume_exceeded: 5,
    start_requested_while_watering: 6,
    stop_requested_for_inactive_zone: 7,
};

export default {
    zigbeeModel: ["IrrigationESP32"],
    model: "IrrigationESP32-6Z",
    vendor: "Open Irrigation",
    description: "Six-zone irrigation controller",
    extend: [
        m.deviceEndpoints({endpoints}),
        m.onOff({endpointNames: Object.keys(endpoints), powerOnBehavior: false}),
        m.deviceAddCustomCluster("irrigationMeasurements", {
            name: "irrigationMeasurements",
            ID: 0xFC00,
            attributes: {
                flowRate: {name: "flowRate", ID: 0x0000, type: Zcl.DataType.SINGLE},
                totalVolumeL: {name: "totalVolumeL", ID: 0x0001, type: Zcl.DataType.SINGLE},
                zoneVolumeL: {name: "zoneVolumeL", ID: 0x0002, type: Zcl.DataType.SINGLE},
                lastCycleVolumeL: {name: "lastCycleVolumeL", ID: 0x0003, type: Zcl.DataType.SINGLE},
                errorCode: {name: "errorCode", ID: 0x0004, type: Zcl.DataType.ENUM8},
            },
            commands: {},
            commandsResponse: {},
        }),
        m.numeric({
            name: "flow_rate",
            cluster: "irrigationMeasurements",
            attribute: "flowRate",
            description: "Current water flow rate",
            unit: "L/min",
            access: "STATE_GET",
            reporting: {min: 1, max: 60, change: 0.05},
            endpointNames: Object.keys(endpoints),
        }),
        m.numeric({
            name: "total_volume_l",
            cluster: "irrigationMeasurements",
            attribute: "totalVolumeL",
            description: "Total water volume across all zones",
            unit: "L",
            access: "STATE_GET",
            reporting: {min: 1, max: 3600, change: 0.1},
            endpointNames: Object.keys(endpoints),
        }),
        m.numeric({
            name: "zone_volume_l",
            cluster: "irrigationMeasurements",
            attribute: "zoneVolumeL",
            description: "Cumulative water volume for this zone",
            unit: "L",
            access: "STATE_GET",
            reporting: {min: 1, max: 3600, change: 0.1},
            endpointNames: Object.keys(endpoints),
        }),
        m.numeric({
            name: "last_cycle_volume_l",
            cluster: "irrigationMeasurements",
            attribute: "lastCycleVolumeL",
            description: "Water volume used during the latest watering cycle",
            unit: "L",
            access: "STATE_GET",
            reporting: {min: 0, max: 3600, change: 0.001},
            endpointNames: Object.keys(endpoints),
        }),
        ...Object.keys(endpoints).map((endpointName) =>
            m.enumLookup({
                name: "irrigation_error",
                cluster: "irrigationMeasurements",
                attribute: "errorCode",
                description: "Most recent irrigation error",
                lookup: irrigationErrors,
                access: "STATE_GET",
                reporting: {min: 0, max: 3600, change: 1},
                endpointName,
            }),
        ),
    ],
};