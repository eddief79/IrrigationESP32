enum class RequestType
{
    StartWatering,
    StopWatering
};

struct IrrigationRequest
{
    RequestType type;
    int zone;
};