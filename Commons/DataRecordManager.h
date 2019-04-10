#ifndef PRIMER_PROYECTO_DATARECORDMANAGER_H
#define PRIMER_PROYECTO_DATARECORDMANAGER_H

template <class DataRecord>
class DataRecordManager {
public:
    virtual void setRecordToResponse(PortalResponse*, DataRecord) = 0;
    virtual DataRecord getRecordFromRequest(MSRequest) = 0;
    virtual INSTANCE_TYPE getServiceName() = 0;
};

class WeatherRecordManager: public DataRecordManager<WeatherRecord> {
public:
    void setRecordToResponse(PortalResponse *portalResponse, WeatherRecord dataRecord) {
        portalResponse->weatherRecord = dataRecord;
    }
    WeatherRecord getRecordFromRequest(MSRequest msRequest) {
        return  msRequest.weatherRecord;
    }
    INSTANCE_TYPE getServiceName() {
        return  WEATHER_MICROSERVICE;
    }
};

class ExchangeRecordManager: public DataRecordManager<ExchangeRecord> {
public:
    void setRecordToResponse(PortalResponse *portalResponse, ExchangeRecord dataRecord) {
        portalResponse->exchangeRecord = dataRecord;
    }
    ExchangeRecord getRecordFromRequest(MSRequest msRequest) {
        return  msRequest.exchangeRecord;
    }
    INSTANCE_TYPE getServiceName() {
        return  EXCHANGE_MICROSERVICE;
    }
};

#endif //PRIMER_PROYECTO_DATARECORDMANAGER_H
