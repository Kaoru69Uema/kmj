#! /usr/bin/env ruby

filename="blink.code"

File.foreach( filename ) do |line|
  p line
end
