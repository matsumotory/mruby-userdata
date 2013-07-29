# Userdata Module for mruby
When shared mrb_state, you can share userdata objects between one Ruby code and another.

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

# License
under the MIT License:

* http://www.opensource.org/licenses/mit-license.php


