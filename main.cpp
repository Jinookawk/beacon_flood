#include <iostream>
#include <unistd.h>
#include <tins/tins.h>

using namespace std;
using namespace Tins;

class Mybeacon {
private:
    Dot11Beacon beacon;
public:
    Mybeacon(){
        beacon.addr1(Dot11::BROADCAST);
        beacon.supported_rates({1.0f, 5.5f, 11.0f});
        beacon.rsn_information(RSNInformation::wpa2_psk());
    }

    void setSSID(char *ssid){
        beacon.ssid(ssid);
    }

    void setChannel(int channel){
        beacon.ds_parameter_set(channel);
    }

    void setMACaddr(HWAddress<6> mac){
        beacon.addr2(mac);
        beacon.addr3(beacon.addr2());
    }

    Dot11Beacon getBeacon(){
        return beacon;
    }
};

int main()
{
    char ssid[3][30]={"Test", "jinookawk", "길길짱잘생김"};
    char mac[3][30]={"00:01:02:03:04:05", "00:32:D7:E8:9F:34", "DE:AD:9F:3B:39:C3"};
    int channel[3]={3, 8, 11};
    class Mybeacon beacon[3];

    for(int i = 0; i < 3; i++){
        beacon[i].setSSID(ssid[i]);
        HWAddress<6> hwaddr(mac[i]);
        beacon[i].setMACaddr(hwaddr);
        beacon[i].setChannel(channel[i]);
    }

    PacketSender sender;
    RadioTap radio1 = RadioTap() / beacon[0].getBeacon();
    RadioTap radio2 = RadioTap() / beacon[1].getBeacon();
    RadioTap radio3 = RadioTap() / beacon[2].getBeacon();
    while(true){
        sender.send(radio1, "wlan1");
        sender.send(radio2, "wlan1");
        sender.send(radio3, "wlan1");
        usleep(100000);
    }
    return 0;
}
