enum class RequestType
{
    StartWatering,
    StopWatering
};

struct IrrigationRequest
{
    RequestType type;
    uint8_t zone;
};