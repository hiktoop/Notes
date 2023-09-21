# Raft

## summary

### State

所有服务器上的持久状态(确保回应 RPC 之前更新状态到磁盘)：

- currentTerm：服务器看到的最新回合

- voteFor：本回合获得选票的候选者 id

- log[]：日志项；每项包含状态机的命令和领导者收到的项的回合

所有服务器上的易变状态：

- commitIndex：已知的将要提交的日志项的 index

- lastApplied：应用到状态机上的最后一个 index

领导者的易变状态：

- nextIndex[]：对于每个服务器将要发送的日志项的 index (初始为领导者最后的日志项 index + 1)

- matchIndex[]：对于每个服务器，已知将要重复的日志项 index (初始为 0 并递增)

### AppendEntries RPC

领导者调用以复制日志项；也作为 heartbeat

Arguments：

- term：领导者的回合

- leaderId：以便追随者可以重定向到客户端

- prevLogIndex：新日志项的前一个日志项 index

- prevLogTerm：prevLogIndex 的日志项的回合

- entries[]：要存储的日志项(heartbeat 时为空；为了效率可能发送多个项)

- leaderCommit：领导者的提交项 index

Results：

- term：现在的回合，以供领导者更新自己

- success：追随者包含的项匹配 prevLogIndex 和 prevLogTerm 则返回 true

Receiver 实现：

1. term < currentTerm 时返回 false

2. 日志不包含 prevLogIndex 的日志项则返回 false

3. 如果存在的项与新的项冲突(相同 index 不同回合)，删除这个项及后面的所有项

4. 本地没有的新日志加到后面

5. leaderCommit > commitIndex 时设置 commitIndex = min(leaderCommit, index of last new entry)

### RequestVote RPC

### Rules for Servers

所有服务器：

- 
