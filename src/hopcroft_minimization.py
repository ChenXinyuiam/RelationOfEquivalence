def hopcroft_minimization(states, alphabet, transitions, initial_state, accepting_states):
    # 步骤一: 初步划分,将所有状态分为 可接受状态 和 非终止状态
    non_accepting = set(states) - set(accepting_states)
    partitions = [set(accepting_states), non_accepting]
    worklist = [set(accepting_states), non_accepting]

    # 步骤二: 进一步划分,找出等价类
    while worklist:
        current = worklist.pop()  # 从worklist中取出一个集合进行划分
        for symbol in alphabet:
            affected = {state for state in states if transitions[state][symbol] in current} # 将所有状态划分为两个部分: affected部分说是满足接受symbol后在同一集合的状态
            new_partitions = []
            for part in partitions:
                intersection = part & affected # 将part进行划分
                difference = part - affected
                if intersection and difference:
                    new_partitions.append(intersection)
                    new_partitions.append(difference)
                    if part in worklist:
                        worklist.remove(part)
                        worklist.append(intersection)
                        worklist.append(difference)
                    else:
                        if len(intersection) <= len(difference): # 后续对difference进行划分后重新加入
                            worklist.append(intersection)
                        else:
                            worklist.append(difference)
                else:
                    new_partitions.append(part)
            partitions = new_partitions # 更新划分

    # 步骤三: 根据划分后的集族和状态转移函数transitions构造新的
    minimized_states = [tuple(part) for part in partitions]
    minimized_transitions = {}
    for part in partitions:
        repr_state = next(iter(part))
        minimized_transitions[tuple(part)] = {symbol: tuple(next(iter(sub_part))
                                   for sub_part in partitions if transitions[repr_state][symbol] in sub_part) for symbol in alphabet}
    return minimized_states, minimized_transitions

states = [0,1,2,3,4]
alphabet = [0,1]
transitions = [[1,2],[0,3],[4,2],[4,3],[4,4]]
initial_state = [0]
accepting_states = [2,3]

minimized_stats, minimized_transitions = hopcroft_minimization(states, alphabet,transitions,initial_state,accepting_states)

print(minimized_stats)

print(minimized_transitions)
