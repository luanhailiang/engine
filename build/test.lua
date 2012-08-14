#!/usr/bin/lua

mongo = require('mongo')
print(123)
-- Create a connection object
local db = assert(mongo.Connection.New())

-- connect to the server on localhost
assert(db:connect('localhost'))

-- insert a value into the namespace 'test.values'
assert(db:insert('test.values', {a = 10, b = 'str1'}))

-- the same using a JSON string
assert(db:insert('test.values', "{'a': 20, 'b': 'str2'}"))

-- insert a multiple values into the namespace 'test.values'
assert(db:insert_batch('test.values', {{a = 10, b = 'str1'}, {c = 11, d = 'str2'}}))

-- print the number of rows in the namespace 'test.values'
print(db:count('test.values'))

-- query all the values in the namespace 'test.values'
local q = assert(db:query('test.values', {}))

-- loop through the result set
for result in q:results() do
        print(result.a)
        print(result.b)
end
