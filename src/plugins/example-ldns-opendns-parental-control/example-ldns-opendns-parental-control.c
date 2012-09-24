
#include <stdio.h>
#include <string.h>

#include <dnscrypt/plugin.h>
#include <ldns/ldns.h>

DCPLUGIN_MAIN(__FILE__);

const char *
dcplugin_description(DCPlugin * const dcplugin)
{
    return "Apply extended OpenDNS parental controls";
}

const char *
dcplugin_long_description(DCPlugin * const dcplugin)
{
    return
        "This enables the following filter set, if one hasn't been defined\n"
        "for the current network or device:\n"
        "- lingerie/bikini\n"
        "- adult themes\n"
        "- nudity\n"
        "- pornography\n"
        "- proxy/anonymizer\n"
        "- sexuality\n"
        "- tasteless";
}

int
dcplugin_init(DCPlugin * const dcplugin, int argc, char *argv[])
{
    return 0;
}

DCPluginSyncFilterResult
dcplugin_sync_pre_filter(DCPlugin *dcplugin, DCPluginDNSPacket *dcp_packet)
{
    uint8_t  *new_packet;
    ldns_rdf *edns_data;
    ldns_pkt *packet;
    size_t    new_packet_size;

    ldns_wire2pkt(&packet, dcplugin_get_wire_data(dcp_packet),
                  dcplugin_get_wire_data_len(dcp_packet));

    edns_data = ldns_rdf_new_frm_str(LDNS_RDF_TYPE_HEX,
                                     "000400084f70656e444e53"
                                     "ff");
    ldns_pkt_set_edns_data(packet, edns_data);

    ldns_pkt2wire(&new_packet, packet, &new_packet_size);
    if (dcplugin_get_wire_data_max_len(dcp_packet) >= new_packet_size) {
        dcplugin_set_wire_data(dcp_packet, new_packet, new_packet_size);
    }

    free(new_packet);
    ldns_pkt_free(packet);

    return DCP_SYNC_FILTER_RESULT_OK;
}