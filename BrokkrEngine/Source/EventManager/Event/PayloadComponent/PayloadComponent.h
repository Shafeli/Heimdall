#pragma once


namespace Brokkr
{
    class Event;

    class PayloadComponent
    {

    public:
        explicit PayloadComponent(Event* pOwner)
            : m_pOwner(pOwner)
        {
            //
        }

        virtual ~PayloadComponent() = default;
        virtual const char* ToString() = 0;

    protected:
        Event* m_pOwner = nullptr;
    };

    class RadioMessagePayload final : public PayloadComponent
    {
        int m_sendersID;
    public:
        RadioMessagePayload(Event* pOwner, int SendingID)
            : PayloadComponent(pOwner)
            , m_sendersID(SendingID)
        {

        }

        int GetSender() const { return m_sendersID; }

        virtual const char* ToString() override;
    };

    class PayloadTest final : public PayloadComponent
    {
    public:
        PayloadTest(Event* pOwner)
            : PayloadComponent(pOwner)
        {

        }

        virtual const char* ToString() override;
    };

    inline const char* RadioMessagePayload::ToString()
    {
        return "RadioMessagePayload\n";
    }

    inline const char* PayloadTest::ToString()
    {
        return "Event Payload Test\n";
    }
}
