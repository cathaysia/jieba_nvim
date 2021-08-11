# 说明

使用 jieba 对中文进行分词，增强了 vim 的 e,b 功能

# 预览

![荷塘月色](./screenshot/荷塘月色.gif)

# 构建
1. cmake -B build -S . -DCMAKE_BUID_TYPE=Release
2. cmake --build build
3. cp build/libjiebamove.so ~/.config/nvim/lua
4. cp build/data/* ~/.config/nvim/lua -r

在 init.lua 中附加以下内容：

```lua
local jieba = require("libjiebamove")

function _G.move_chs(isRight)
    local row = vim.api.nvim_win_get_cursor(0)[1]
    local col = vim.api.nvim_win_get_cursor(0)[2]
    local content = vim.api.nvim_buf_get_lines(0,row-1,row,0)[1]
    local new_pos = jieba.getPos(content, col, isRight)
    print(new_pos)
    vim.api.nvim_win_set_cursor(0,{row,new_pos})
end


vim.api.nvim_set_keymap("n","e", ":lua move_chs(1)<CR>", {})
vim.api.nvim_set_keymap("n","b", ":lua move_chs(0)<CR>", {})
```


