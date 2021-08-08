> 注意：这只是一个简单的库，它使用 nvim 的 lua 部分 + jieba 组成，旨在为我或者其他人提供一个原型

根据结巴分词来移动 vim 中的光标

使用方式：

- 下载 py 文件到 /bin 路径
- 使用 pip 安装结巴分词
- 在 vim 的初始化脚本中添加以下内容：

```lua
Lua<<
local encodeURI = function (s)
    s = string.gsub(s, "([^%w%.%- ])", function(c) return string.format("%%%02X", string.byte(c)) end)
    return string.gsub(s, " ", "+")
end

function _G.move_chs(direction)
    local row = vim.api.nvim_win_get_cursor(0)[1]
    local col = vim.api.nvim_win_get_cursor(0)[2]
    local content = vim.api.nvim_buf_get_lines(0,row-1,row,0)[1]
    content = encodeURI(content)
    local cmd = string.format("python /bin/vim_jieba.py %s %d %s 2>/dev/null",content, col, direction)
    local new_pos = tonumber(io.popen(cmd):read("*l"))
    vim.api.nvim_win_set_cursor(0,{row,new_pos})
end

vim.api.nvim_set_keymap("n","<Leader>lr", ":lua move_chs('right')<CR>", {})
vim.api.nvim_set_keymap("n","<Leader>ll", ":lua move_chs('left')<CR>", {})
EOF
```

现在，重启你的 vim ，然后使用 <Leader>lr 向右移动光标，使用 <Leader>ll 向左移动官标

结巴分词比较慢，而且 Lua 调用 py 比较麻烦，有空的话我会重新用 C++ 写一个插件
