# Getting Started
1. Run `make`. This will compile all Java files.
2. Open 8 seperate terminals. To see which terminals to open, head over to the **clientInfos** and **serverInfos** files.
3. Server Setup
    - Run `java Server <id>` for ids **0-2** in 3 seperate terminals
    - In increasing order of ServerID terminals, type `setup` and press **Enter**.
4. Client Setup
    - Run `java Client <id>` for ids **0-4** in 5 seperate terminals.
    - You should see messages populating on client and server terminals.
5. Once clients and servers are setup and connected to each other, type `run` in each of the client terminals, which will auto-generate requests to the servers from the client.
6. Check the server directories to see if all messages are populated exactly as they should be.

# Tar Commands
- **Unzip**: `tar -xvf sxp170022_p2.tar.gz`
- **Look Inside**: `tar -tvf sxp170022_p2.tar.gz`