" All system-wide defaults are set in $VIMRUNTIME/debian.vim and sourced by
" the call to :runtime you can find below.  If you wish to change any of those
" settings, you should do it in this file (/etc/vim/vimrc), since debian.vim
" will be overwritten everytime an upgrade of the vim packages is performed.
" It is recommended to make changes after sourcing debian.vim since it alters
" the value of the 'compatible' option.

" This line should not be removed as it ensures that various options are
" properly set to work with the Vim-related packages available in Debian.
runtime! debian.vim

" Uncomment the next line to make Vim more Vi-compatible
" NOTE: debian.vim sets 'nocompatible'.  Setting 'compatible' changes numerous
" options, so any other options should be set AFTER setting 'compatible'.
"set compatible

" Vim5 and later versions support syntax highlighting. Uncommenting the next
" line enables syntax highlighting by default.
if has("syntax")
  syntax on
endif

" If using a dark background within the editing area and syntax highlighting
" turn on this option as well
"set background=dark

" Uncomment the following to have Vim jump to the last position when
" reopening a file
if has("autocmd")
  au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
endif

" Uncomment the following to have Vim load indentation rules and plugins
" according to the detected filetype.
if has("autocmd")
  filetype plugin indent on
endif

" The following are commented out as they cause vim to behave a lot
" differently from regular Vi. They are highly recommended though.
set showcmd		" Show (partial) command in status line.
set showmatch		" Show matching brackets.
"set ignorecase		" Do case insensitive matching
"set smartcase		" Do smart case matching
set incsearch		" Incremental search
set autowrite		" Automatically save before commands like :next and :make
"set hidden		" Hide buffers when they are abandoned
set mouse=a		" Enable mouse usage (all modes)

" Source a global configuration file if available
if filereadable("/etc/vim/vimrc.local")
  source /etc/vim/vimrc.local
endif

set autoindent
set nu
set sw=4
set ts=4
set hlsearch
set fileencodings=utf-8,ucs-bom,gb18030,gbk,gb2312,cp936
set termencoding=utf-8
set encoding=utf-8

let Tlist_Inc_Winwidth=0

" Use Vundle to manage the plugins
set nocompatible
filetype off
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" let Vundle manage itself
Plugin 'VundleVim/Vundle.vim'

" plugins
Plugin 'Auto-Pairs'
Plugin 'vim-scripts/taglist.vim'
Plugin 'Valloric/YouCompleteMe'

call vundle#end()

filetype plugin indent on
func! CompileGcc()
     exec "w"
     let compilecmd="!gcc "
     let compileflag="-o %< "
     if search("mpi\.h") != 0
         let compilecmd = "!mpicc "
     endif
     if search("glut\.h") != 0
         let compileflag .= " -lglut -lGLU -lGL "
     endif
     if search("cv\.h") != 0
         let compileflag .= " -lcv -lhighgui -lcvaux "
     endif
     if search("omp\.h") != 0
         let compileflag .= " -fopenmp "
     endif
     if search("math\.h") != 0
         let compileflag .= " -lm "
     endif
     exec compilecmd." % ".compileflag
 endfunc
func! CompileGpp()
     exec "w"
     let compilecmd="!g++ "
     let compileflag="-std=c++11 "
     let compileflag.="-o %< "
     if search("mpi\.h") != 0
         let compilecmd = "!mpic++ "
     endif
     if search("glut\.h") != 0
         let compileflag .= " -lglut -lGLU -lGL "
     endif
     if search("cv\.h") != 0
         let compileflag .= " -lcv -lhighgui -lcvaux "
     endif
     if search("omp\.h") != 0
         let compileflag .= " -fopenmp "
     endif
     if search("math\.h") != 0
         let compileflag .= " -lm "
     endif
     exec compilecmd." % ".compileflag
 endfunc

func! RunPython()
         exec "!python %"
 endfunc
func! CompileJava()
     exec "!javac %"
 endfunc


func! CompileCode()
         exec "w"
         if &filetype == "cpp"
                 exec "call CompileGpp()"
		 exec "!./%<"
         elseif &filetype == "c"
                 exec "call CompileGcc()"
		 exec "!./%<"
         elseif &filetype == "python"
                 exec "call RunPython()"
             	 exec "call RunPython"
         elseif &filetype == "java"
                 exec "call CompileJava()"
             	 exec "!java %<"
         endif
 endfunc

 map <F5> :call CompileCode()<CR>
 imap <F5> <ESC>:call CompileCode()<CR>
 vmap <F5> <ESC>:call CompileCode()<CR>

 " Add information by WuYang
map <F4> ms:call AddAuthor()<cr>'S

function AddAuthor()
    let n=1
    while n < 11
        let line = getline(n)
        if line=~'[#]*\s*\*\s*\S*Last\s*modified\s*:\s*\S*.*$'
        call UpdateTitle()
        return
    endif
    let n = n + 1
    endwhile
    if &filetype == 'sh'
        call AddTitleForShell()
    elseif &filetype == 'python'
        call AddTitleForPython()
    else
        call AddTitleForC()
    endif

endfunction


function UpdateTitle()
    normal m'
    execute '/* Last modified\s*:/s@:.*$@\=strftime(": %Y-%m-%d %H:%M")@'
    normal mk
    execute '/* Filename\s*:/s@:.*$@\=": ".expand("%:t")@'
    execute "noh"
    normal 'k
    echohl WarningMsg | echo "Successful in updating the copy right." |echohl None
endfunction

function AddTitleForC()
    call append(0,"/----------------------------------------------------------")
    call append(1," * Author        : WuYang")
    call append(2," * Email         : yangwuxd@163.com")
    call append(3," * Create time   : ".strftime("%Y-%m-%d %H:%M"))
    call append(4," * Last modified : ".strftime("%Y-%m-%d %H:%M"))
    call append(5," * Filename      : ".expand("%:t"))
    call append(6," * Description   : ")
    call append(7," * -------------------------------------------------------/")
    echohl WarningMsg | echo "Successful in adding the copyright." | echohl None
endfunction

"" add comment for Python
function AddTitleForPython()
    call append(0,"#!/usr/bin/python")
    call append(1,"# -*- coding: UTF-8 -*-")
    call append(2,"")
    call append(3,"# **********************************************************")
    call append(4,"# * Author        : WuYang")
    call append(5,"# * Email         : yangwuxd@163.com")
    call append(6,"# * Create time   : ".strftime("%Y-%m-%d %H:%M"))
    call append(7,"# * Last modified : ".strftime("%Y-%m-%d %H:%M"))
    call append(8,"# * Filename      : ".expand("%:t"))
    call append(9,"# * Description   : ")
    call append(10,"# **********************************************************")
    echohl WarningMsg | echo "Successful in adding the copyright." | echohl None
endfunction

"" add conment for shell
function AddTitleForShell()
    call append(0,"#!/bin/bash")
    call append(1,"# **********************************************************")
    call append(2,"# * Author        : WuYang")
    call append(3,"# * Email         : yangwuxd@163.com")
    call append(4,"# * Create time   : ".strftime("%Y-%m-%d %H:%M"))
    call append(5,"# * Last modified : ".strftime("%Y-%m-%d %H:%M"))
    call append(6,"# * Filename      : ".expand("%:t"))
    call append(7,"# * Description   : ")
    call append(8,"# **********************************************************")
endfunction

"open taglist
nnoremap <silent> <F8> :TlistToggle<CR>

" #####YouCompleteMe Configure 
let g:ycm_global_ycm_extra_conf = '~/.ycm_extra_conf.py'
" 自动补全配置
set completeopt=longest,menu	"让Vim的补全菜单行为与一般IDE一致(参考VimTip1228)
autocmd InsertLeave * if pumvisible() == 0|pclose|endif	"离开插入模式后自动关闭预览窗口
inoremap <expr> <CR>       pumvisible() ? "\<C-y>" : "\<CR>"	"回车即选中当前项
"上下左右键的行为 会显示其他信息
inoremap <expr> <Down>     pumvisible() ? "\<C-n>" : "\<Down>"
inoremap <expr> <Up>       pumvisible() ? "\<C-p>" : "\<Up>"
inoremap <expr> <PageDown> pumvisible() ? "\<PageDown>\<C-p>\<C-n>" : "\<PageDown>"
inoremap <expr> <PageUp>   pumvisible() ? "\<PageUp>\<C-p>\<C-n>" : "\<PageUp>"

"youcompleteme  默认tab  s-tab 和自动补全冲突
"let g:ycm_key_list_select_completion=['<c-n>']
"let g:ycm_key_list_select_completion = ['<Down>']
"let g:ycm_key_list_previous_completion=['<c-p>']
"let g:ycm_key_list_previous_completion = ['<Up>']
let g:ycm_confirm_extra_conf=0 "关闭加载.ycm_extra_conf.py提示

let g:ycm_collect_identifiers_from_tags_files=1	" 开启 YCM 基于标签引擎
let g:ycm_min_num_of_chars_for_completion=2	" 从第2个键入字符就开始罗列匹配项
let g:ycm_cache_omnifunc=0	" 禁止缓存匹配项,每次都重新生成匹配项
let g:ycm_seed_identifiers_with_syntax=1	" 语法关键字补全
nnoremap <F5> :YcmForceCompileAndDiagnostics<CR>	"force recomile with syntastic
"nnoremap <leader>lo :lopen<CR>	"open locationlist
"nnoremap <leader>lc :lclose<CR>	"close locationlist
inoremap <leader><leader> <C-x><C-o>
"在注释输入中也能补全
"let g:ycm_complete_in_comments = 1
"在字符串输入中也能补全
"let g:ycm_complete_in_strings = 1
"注释和字符串中的文字也会被收入补全
"let g:ycm_collect_identifiers_from_comments_and_strings = 0
let g:clang_user_options='|| exit 0'
nnoremap <leader>jd :YcmCompleter GoToDefinitionElseDeclaration<CR> " 跳转到定义处
" #####YouCompleteMe Configure 

let g:ycm_key_invoke_completion='<C-a>'
