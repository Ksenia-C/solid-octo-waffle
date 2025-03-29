poems_data = [
    ('id1', 'Alice in the tree', 14000, "Wonderson"),
    ('id2', 'Bob outside the top', 15000, "Sleepson"),
    ('id3', 'Sandy and a candy', 4000, "Davidson"),
    ('id4', 'Cry out loud', 84000, "Coolson"),
    ('id5', 'The big liar', 19000, "Cupson"),
    ('id6', 'who wants A+ say it out loud', 100000, "Zanzibar gem"),
    ('id7', 'we dont believe whats on tv', 500, "21 pilots"),
    ('id8', 'the mind electric', 10, "Miracle Musical"),
]

novels_data = [
    ('The good function', 2015, "Wonderson"),
    ('Plants', 2000, "Sleepson"),
    ('dirty reads and writes', 2121, "Davidson"),
    ('please water me', 1923, "Zanzibar gem"),
    ('migraine', 2002, "21 pilots"),
    ('Labyrinth', 2013, "Miracle Musical"),
]


reviews = [
    ('Alice in the tree', 9),
    ('Bob outside the top', 6),
    ('Sandy and a candy', 5),
    ('Cry out loud', 10),
    ('The big liar', 9),
    ('who wants A+ say it out loud', 3),
    ('we dont believe whats on tv', 9),
    ('the mind electric', 10),
    ('The good function', 4),
    ('Plants', 10),
    ('dirty reads and writes', 6),
    ('please water me', 2),
    ('migraine', 9),
    ('Labyrinth', 9),
]


reviews_kv = {}
for review in reviews:
    reviews_kv[review[0]] = review[1]

authors_with_reviews = {}
for poem in poems_data:
    authors_with_reviews[poem[3]] = authors_with_reviews.get(poem[3], 0) + reviews_kv[poem[1]]

for novel in novels_data:
    authors_with_reviews[novel[2]] = authors_with_reviews.get(novel[2], 0) + reviews_kv[novel[0]]



print("The author with the highest rate", sorted(authors_with_reviews.items(), key=lambda x: x[1])[:-1])
print("All authors", len(authors_with_reviews))
