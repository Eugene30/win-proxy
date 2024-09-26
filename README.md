# Win-proxy
Simple tool that lets you configure in-built system proxy. Windows platform only. Lets you enable or disable proxy and set its address/port. You can always tweak proxy settings manually in "Control panel -> Network -> Proxy".

# Arguments
Executable requires at least one or two arguments provided:
- Enable : enable or disable proxy.
- Protocol and address : used protocol and address in IP:port format

# Usage
Some usage examples:

```
proxy.exe 1 socks=127.0.0.1:9050 - enable proxy on socks protocol and provided address/port
proxy.exe 0                      - disable proxy
```
