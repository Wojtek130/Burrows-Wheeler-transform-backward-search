# BANANA$
# BACACA$ <- minimal letters
# ACCB$AA <- bwt
# (A, 0), (C, 0), (C, 1), (B, 0), ($, 0), (A, 1), (A, 2) <- letter indices added
# ($, 0), (A, 0), (A, 1), (A, 2), (B, 0), (C, 0), (C, 1),  <- first column
# prefix sums: {$: 0, A: 1, B: 4, C: 5}
# start = 0, end = n-1
# first iter: start = prefix_sums[letter[0]], end = prefix_sums[letter[0] + 1] - 1
# next  iter: start = prefix_sums[letter[0]] + bwt[a][1], end = prefix_sums[letter[0]] + bwt[b][1] - 1
# after loop: 

def minimal_alphabet_dicts(s):
    unique_letters = sorted(set(s))
    minimal_alphabet = []
    minimal_alphabet_encode = {}
    minimal_alphabet_decode = {}
    letter_to_use = "$"
    for i, c in enumerate(unique_letters):
        minimal_alphabet_encode[c] = letter_to_use
        minimal_alphabet_decode[letter_to_use] = c
        minimal_alphabet.append(letter_to_use)
        if i == 0:
            letter_to_use = "a"
        else:
            letter_to_use = chr(ord(letter_to_use) + 1)
    return minimal_alphabet_encode, minimal_alphabet_decode, minimal_alphabet

def minimal_string_encoding(s, minimal_alphabet_encode):
    new_s = ""
    for c in s:
        try:
            new_s += minimal_alphabet_encode[c]
        except KeyError:
            print(f"Character '{c}' not in minimal alphabet")
    return new_s

def decode_string(s, minimal_alphabet_decode):
    new_s = ""
    for c in s:
        new_s += minimal_alphabet_decode[c]
    return new_s

def suffix_array_construction(s):
    """Construct the suffix array of a string s in linear time."""
    n = len(s)
    suffix_array = list(range(n))
    rank = list(map(ord, s))  # Initial ranks based on character ASCII values
    k = 1
    while k < n:
        key = lambda x: (rank[x], rank[x + k] if x + k < n else -1)
        suffix_array.sort(key=key)  # Sort suffix indices by (rank, rank + k)
        tmp = [0] * n
        for i in range(1, n):
            tmp[suffix_array[i]] = tmp[suffix_array[i - 1]]
            if key(suffix_array[i]) != key(suffix_array[i - 1]):
                tmp[suffix_array[i]] += 1
        rank = tmp
        k *= 2
    return suffix_array

def burrows_wheeler_transform(s):
    """Compute the Burrows-Wheeler Transform of a string s in linear time."""
    suffix_array = suffix_array_construction(s)
    bwt = "".join(s[i - 1] if i > 0 else "$" for i in suffix_array)
    return bwt

def letter_occurences_and_indexed_bwt(bwt):
    letters_count = {}
    indexed_bwt = []
    for c in bwt:
        if c not in letters_count:
            letters_count[c] = 0
        indexed_bwt.append((c, letters_count[c]))
        letters_count[c] += 1
    return letters_count, indexed_bwt

def compute_prefix_sums(letter_count, alphabet):
    previous_sum = 0
    prefix_sums = {}
    for c in alphabet:
        prefix_sums[c] = previous_sum
        previous_sum += letter_count[c]
    return prefix_sums

def occ(letter, bwt, i):
    # O(n) but should be O(1)
    return bwt[:(i+1)].count(letter)

def calculate_rank(bwt, alphabet):
    r = {}
    n = len(bwt)
    for c in alphabet:
        c_occ = [None] * n
        counter = 0
        for i in range(n):
            if bwt[i] == c:
                counter += 1
            c_occ[i] = counter
        r[c] = c_occ
    return r


def get_next_prefix_sum(prefix_sums, letter, n, last_char_in_alphabet):
    next_sum = 0
    if letter == "$":
        next_sum = prefix_sums["a"]
    elif letter == last_char_in_alphabet: 
        next_sum = n
    else:
        next_sum = prefix_sums[chr(ord(letter) + 1)]
    return next_sum

def backwards_search(p_enc, indexed_bwt, prefix_sums, last_char_in_alphabet, rank):
    n = len(indexed_bwt)
    a, b = 0, 0
    for i, letter in enumerate(p_enc[::-1]):
        if i == 0:
            a = prefix_sums[letter]
            next_sum = get_next_prefix_sum(prefix_sums, letter, n, last_char_in_alphabet)
            b = next_sum - 1
            # if letter == "$":
            #     b = prefix_sums["a"] - 1
            # elif letter == last_char_in_alphabet: 
            #     b = n - 1
            # else:
            #     b = prefix_sums[chr(ord(letter) + 1)] - 1
        else:
            a = prefix_sums[letter] + rank[letter][a-1]
            b = prefix_sums[letter] + rank[letter][b] - 1
        print(a, b)
        if b < a:
            return f"no pattern found"
    return a, b

def reverse_indexed_bwt(indexed_bwt, prefix_sums):
    n = len(indexed_bwt)
    indexed_s = []
    i = 0
    current_letter = ("$", 0) 
    for _ in range(n):
        print(i, current_letter)
        indexed_s.append(current_letter)
        current_letter = indexed_bwt[i]
        i = prefix_sums[current_letter[0]] + current_letter[1]
    indexed_s.reverse()
    return indexed_s

def start_letter_occurences(start, end, f):
    return f[start:end+1]
    # occurrences = []
    # print(start, end, "!!!")
    # for i in range(start, end + 1):
    #     next_sum = get_next_prefix_sum(prefix_sums, pattern_first_letter, n, last_char_in_alphabet)
    #     # print(prefix_sums[pattern_first_letter] + rank[pattern_first_letter][i], i - "-----")
    #     # new_ind = prefix_sums[pattern_first_letter] + rank[pattern_first_letter][i-1]
    #     # print(new_ind, indexed_bwt[new_ind], rank[pattern_first_letter][i], "-----")
    #     if indexed_bwt[i][0] == pattern_first_letter:
    #         occurrences.append(i)
    # return occurrences
    # return [(pattern_first_letter, i-1) for i in range(start, end + 1)]

def initial_string_occurences(indexed_s, start_letter_occurences):
    print(indexed_s, "!!!!", start_letter_occurences)
    return [ind for ind, indexed_l in enumerate(indexed_s) if indexed_l in start_letter_occurences]

def check_occurences(initial_s_occurences, s, p):
    m = len(p)
    return [s[i:i+m] for i in initial_s_occurences]

def first_column(indexed_bwt):
    return sorted(indexed_bwt)

s = "banana$"
n = len(s)
minimal_alphabet_encode, minimal_alphabet_decode, alphabet = minimal_alphabet_dicts(s)
s_enc = minimal_string_encoding(s, minimal_alphabet_encode)
bwt = burrows_wheeler_transform(s_enc)
print(bwt) 
letters_count, indexed_bwt = letter_occurences_and_indexed_bwt(bwt)
print(indexed_bwt, "indexed_bwt")
print(letters_count)
print(alphabet)
prefix_sums = compute_prefix_sums(letters_count, alphabet)
print(prefix_sums)
p = "bana"
p_enc = minimal_string_encoding(p, minimal_alphabet_encode)
print(p_enc)
rank = calculate_rank(bwt, alphabet)
print(rank)
print(backwards_search(p_enc, indexed_bwt, prefix_sums, alphabet[-1], rank))
start, end = backwards_search(p_enc, indexed_bwt, prefix_sums, alphabet[-1], rank)
print(bwt, start, end)
indexed_s = reverse_indexed_bwt(indexed_bwt, prefix_sums)
f = first_column(indexed_bwt)
start_l_occ = start_letter_occurences(start, end, f)
print(start_l_occ)
final_occ = initial_string_occurences(indexed_s, start_l_occ)
print(final_occ)
print(check_occurences(final_occ, s, p))
print(rank["a"], prefix_sums)

