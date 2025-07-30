#pragma once

namespace ipc
{
class IShmChannelNotify
{
public:
    IShmChannelNotify() {}
    virtual ~IShmChannelNotify() {}

    virtual void OnDataRecived(const char* buf, int size) = 0;
    virtual void OnWriteSuccess(int id) = 0;
    virtual void OnWriteFailed(int id) = 0;

private:
    IShmChannelNotify(const IShmChannelNotify&) = delete;
    IShmChannelNotify& operator=(const IShmChannelNotify&) = delete;
};

}  // namespace ipc
