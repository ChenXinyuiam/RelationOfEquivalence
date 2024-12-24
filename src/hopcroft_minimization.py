def hopcroft_minimization(states, alphabet, transitions, initial_state, accepting_states):
    # Step 1: Initial partition
    non_accepting = set(states) - set(accepting_states)
    partitions = [set(accepting_states), non_accepting]
    worklist = [set(accepting_states), non_accepting]

    # Step 2: Refine partitions using worklist
    while worklist:
        current = worklist.pop()
        for symbol in alphabet:
            affected = {state for state in states if transitions[state][symbol] in current}
            new_partitions = []
            for part in partitions:
                intersection = part & affected
                difference = part - affected
                if intersection and difference:
                    new_partitions.append(intersection)
                    new_partitions.append(difference)
                    if part in worklist:
                        worklist.remove(part)
                        worklist.append(intersection)
                        worklist.append(difference)
                    else:
                        if len(intersection) <= len(difference):
                            worklist.append(intersection)
                        else:
                            worklist.append(difference)
                else:
                    new_partitions.append(part)
            partitions = new_partitions

    # Step 3: Create minimized DFA
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
