#pragma once
#include "MeshModule.h"

/**
 * A simple example module that just replies with "Message received" to any message it receives.
 */
class ExtensionModule : public MeshModule
{
  private:


  public:
    /** 
     * Constructor
     * name is for debugging output
     */
    //ExtensionModule();
    ExtensionModule();

    virtual bool wantPacket(const meshtastic_MeshPacket *p) override {
        return true; // Recive all packets
    }

  protected:
    ProcessMessage handleReceived(const meshtastic_MeshPacket &mp) override; 

  private:
    void bot(const meshtastic_MeshPacket &mp);

};