# Userdata class for mruby
When shared mrb_state, you can share userdata objects between one Ruby code and the other.

## install by mrbgems
 - add conf.gem line to `build_config.rb`
```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :git => 'https://github.com/matsumoto-r/mruby-userdata.git'
end
```

## How to use
 - One ruby code A

    ```ruby
    u = Userdata.new
    u.hoge = {:hoge => 1}
    ```


 - another ruby code B is shared mrb_state of ruby code A

    ```ruby
    u = Userdata.new
    hash = u.hoge
    hash[:fuga] = 2
    u.hoge = hash
    
    p u.hoge # => {:hoge => 1, :fuga =>2}
    ```

 - use userdata key

    ```ruby
    mirb - Embeddable Interactive Ruby Shell
    
    This is a very early version, please test and report errors.
    Thanks :)
    
    > u = Userdata.new
     => #<Userdata:0x7f96ca808ad0 $mrb_userdata_key_store="$mrb_userdata_default_key">
    > u.hoge
     => nil
    > u.hoge = 1
     => 1
    > u.hoge
     => 1
    > q = Userdata.new
     => #<Userdata:0x7f96ca808590 $mrb_userdata_key_store="$mrb_userdata_default_key">
    > q.hoge
     => 1
    > q.hoge = q.hoge + 1
     => 2
    > u.hoge
     => 2
    > s = Userdata.new "my_key"
     => #<Userdata:0x7f96ca807d80 $mrb_userdata_key_store="my_key">
    > s.hoge
     => nil
    > s.hoge = 1
     => 1
    > u.hoge
     => 2
    > q.hoge
     => 2
    ```

# License
under the MIT License:

* http://www.opensource.org/licenses/mit-license.php


