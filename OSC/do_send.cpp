#include <stdio.h>
#include "OscOutboundPacketStream.h"
#include "UdpSocket.h"
namespace osc{

BundleInitiator BeginBundleImmediate(1);
BundleTerminator EndBundle;
MessageTerminator EndMessage;
NilType Nil;
InfinitumType Infinitum;

} // namespace osc

#define OUTPUT_BUFFER_SIZE 1024
extern "C" void do_send(char *host, int port, char *addr, double vals[], int cnt)
{
    int i;
    UdpTransmitSocket transmitSocket(IpEndpointName(host, port));
    char buffer[OUTPUT_BUFFER_SIZE];
    //    printf("%s %d %s\n", host, port, addr);
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );
    p << osc::BeginMessage(addr);
    for (i=0; i<cnt; i++) p << vals[i];
    p << osc::EndMessage;
    transmitSocket.Send(p.Data(), p.Size());
}
