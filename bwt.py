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

def radix_sort(s, alphabet):
    n = len(alphabet)
    char_to_index = {char: i for i, char in enumerate(alphabet)}  
    count = [0] * n
    for char in s:
        count[char_to_index[char]] += 1
    sorted_string = []
    for i, freq in enumerate(count):
        sorted_string.append(alphabet[i] * freq) 
    return list(filter(lambda x: x != "", sorted_string))

def construct_minimal_alphabet(s, initial_alphabet="$abcdefghijklmnopqrstuvwxyz"):
    unique_letters = radix_sort(set(s), initial_alphabet)
    minimal_alphabet = []
    minimal_alphabet_encode = {}
    letter_to_use = "$"
    for i, c in enumerate(unique_letters):
        minimal_alphabet_encode[c] = letter_to_use
        minimal_alphabet.append(letter_to_use)
        if i == 0:
            letter_to_use = "a"
        else:
            letter_to_use = chr(ord(letter_to_use) + 1)
    return minimal_alphabet_encode, minimal_alphabet

def minimal_string_encoding(s, minimal_alphabet_encode):
    new_s = ""
    for c in s:
        try:
            new_s += minimal_alphabet_encode[c]
        except KeyError:
            print(f"Character '{c}' not in minimal alphabet")
            return None
    return new_s

def decode_string(s, minimal_alphabet_decode):
    new_s = ""
    for c in s:
        new_s += minimal_alphabet_decode[c]
    return new_s

def suffix_array_construction(s):
    n = len(s)
    suffix_array = list(range(n))
    rank = list(map(ord, s))  # Initial ranks based on character ASCII values
    k = 1
    while k < n:
        key = lambda x: (rank[x], rank[x + k] if x + k < n else -1)
        suffix_array.sort(key=key)  
        tmp = [0] * n
        for i in range(1, n):
            tmp[suffix_array[i]] = tmp[suffix_array[i - 1]]
            if key(suffix_array[i]) != key(suffix_array[i - 1]):
                tmp[suffix_array[i]] += 1
        rank = tmp
        k *= 2
    return suffix_array

def burrows_wheeler_transform(s):
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
        else:
            a = prefix_sums[letter] + rank[letter][a-1]
            b = prefix_sums[letter] + rank[letter][b] - 1
        if b < a:
            return None, None
    return a, b

def reverse_indexed_bwt(indexed_bwt, prefix_sums):
    n = len(indexed_bwt)
    indexed_s = []
    i = 0
    current_letter = ("$", 0) 
    for _ in range(n):
        indexed_s.append(current_letter)
        current_letter = indexed_bwt[i]
        i = prefix_sums[current_letter[0]] + current_letter[1]
    indexed_s.reverse()
    return indexed_s

def start_letter_occurences(start, end, first_pattern_letter, prefix_sums):
    diff = end - start + 1
    return [(first_pattern_letter, start - prefix_sums[first_pattern_letter] + i) for i in range(diff)]

def initial_string_occurences(indexed_s, start_letter_occurences):
    return [ind for ind, indexed_l in enumerate(indexed_s) if indexed_l in start_letter_occurences]

def check_occurences(initial_s_occurences, s, p):
    m = len(p)
    return [s[i:i+m] for i in initial_s_occurences]

def bwt_search(s_org, p):
    s = s_org + "$"
    minimal_alphabet_encode, alphabet = construct_minimal_alphabet(s)
    print("minimal alphabet encode: ", minimal_alphabet_encode)
    print("alphabet", alphabet)
    s_enc = minimal_string_encoding(s, minimal_alphabet_encode)
    p_enc = minimal_string_encoding(p, minimal_alphabet_encode)
    print("encoded string: ", s_enc)
    print("encoded pattern: ", p_enc)
    if p_enc is None:
        print(f"Pattern '{p}' not found in '{s_org}'")
        return
    bwt = burrows_wheeler_transform(s_enc)
    print("bwt: ", bwt)
    letters_count, indexed_bwt = letter_occurences_and_indexed_bwt(bwt)
    print("letters count: ", letters_count)
    print("indexed bwt: ", indexed_bwt)
    prefix_sums = compute_prefix_sums(letters_count, alphabet)
    print("prefix sums: ", prefix_sums)
    rank = calculate_rank(bwt, alphabet)
    print("rank: ", rank)
    start, end = backwards_search(p_enc, indexed_bwt, prefix_sums, alphabet[-1], rank)
    print("start: ", start)
    print("end: ", end)
    if start is None or end is None:
        print(f"Pattern '{p}' not found in '{s_org}'")
        return
    indexed_s = reverse_indexed_bwt(indexed_bwt, prefix_sums)
    print("indexed s: ", indexed_s)
    start_l_occ = start_letter_occurences(start, end, p_enc[0], prefix_sums)
    print("start_l_ocss: ", start_l_occ)
    final_occ = initial_string_occurences(indexed_s, start_l_occ)
    print("final_occ", final_occ)
    print(check_occurences(final_occ, s, p))
    print("sa", suffix_array_construction(s_enc))

s = "banana"
p = "an"
bwt_search(s, p)
