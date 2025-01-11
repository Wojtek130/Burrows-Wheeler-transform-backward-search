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
    minimal_alphabet_encode = {}
    minimal_alphabet_decode = {}
    letter_to_use = "a"
    for c in unique_letters:
        minimal_alphabet_encode[c] = letter_to_use
        minimal_alphabet_decode[letter_to_use] = c
        letter_to_use = chr(ord(letter_to_use) + 1)
    return minimal_alphabet_encode, minimal_alphabet_decode

def minimal_string_encoding(s, minimal_alphabet_encode):
    new_s = ""
    for c in s:
        new_s += minimal_alphabet_encode[c]
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



s = "steak"
s = 'abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz'
minimal_alphabet_encode, minimal_alphabet_decode = minimal_alphabet_dicts(s)
print(minimal_alphabet_encode)
encoded_s = minimal_string_encoding(s, minimal_alphabet_encode)
decoded_s = decode_string(encoded_s, minimal_alphabet_decode)
print(s, encoded_s, decoded_s)
print(s == decoded_s == encoded_s)
# print(decode_string(s, minimal_alphabet_decode))



# print(minimal_letters('steak'))
