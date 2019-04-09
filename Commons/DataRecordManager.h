#ifndef PRIMER_PROYECTO_DATARECORDMANAGER_H
#define PRIMER_PROYECTO_DATARECORDMANAGER_H

template <class DataRecord>
class DataRecordManager {
public:
    virtual void setRecordToResponse(PortalResponse*, DataRecord) = 0;
    virtual DataRecord getRecordFromRequest(MSRequest) = 0;
    virtual Service getServiceName() = 0;
};

class WeatherRecordManager: public DataRecordManager<WeatherRecord> {
public:
    void setRecordToResponse(PortalResponse *portalResponse, WeatherRecord dataRecord) {
        portalResponse->weatherRecord = dataRecord;
    }
    WeatherRecord getRecordFromRequest(MSRequest msRequest) {
        return  msRequest.weatherRecord;
    }
    Service getServiceName() {
        return  WEATHER;
    }
};

class ExchangeRecordRecordManager: public DataRecordManager<ExchangeRecord> {
public:
    void setRecordToResponse(PortalResponse *portalResponse, ExchangeRecord dataRecord) {
        portalResponse->exchangeRecord = dataRecord;
    }
    ExchangeRecord getRecordFromRequest(MSRequest msRequest) {
        return  msRequest.exchangeRecord;
    }
    Service getServiceName() {
        return  EXCHANGE;
    }
};

#endif //PRIMER_PROYECTO_DATARECORDMANAGER_H
