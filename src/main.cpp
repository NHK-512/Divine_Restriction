SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);

    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message* message)
    {

        if (message->type == SKSE::MessagingInterface::kDataLoaded)
        {
            REX::INFO("Data Loaded");
            MainHook::Hook();
        }
    });

	return true;
}
