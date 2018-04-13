::: template
::: ignore EOL INDENT DEDENT


:: parse
    Program:
        stmt => s:GlobalStatement z:Sep next:Program
        end => s:GlobalStatement z:Sep
    LocalProgram!
        stmt => s:LocalStatement z:Sep next:LocalProgram
        end => s:LocalStatement z:Sep

    Sep!
        sep => ";" s:Sep
        none => NONE

    Block!
        body => "{" body:LocalProgram "}"
        empty => "{""}"

    GlobalStatement!
        p => s:With
        p => s:Macro
        p => s:Impl
        p => s:Function
        s => s:Struct
        s => s:Enum
        s => s:Union
        s => s:Definition
        s => s:Declaration
        s => s:Expr

    LocalStatement!
        p => s:With
        p => s:Macro
        p => s:If
        p => s:Match
        s => s:Loop
        s => s:Skip
        s => s:Break
        s => s:Return
        s => s:Definition
        s => s:Declaration
        s => s:Expr

    Macro! => "macro"":" macro:STRING

    Loop!
        named => name:Named "loop" body:Block
        anon => "loop" body:Block
    Break!
        named => "break" name:Name
        anon => "break"
    Skip!
        named => "skip" name:Name
        anon => "skip"

    If! => "if" cond:Expr body:Block other:Else
    Else!
        else => "else" body:If
        else => "else" body:Block
        none => NONE

    Match! => "match" cond:Expr "{" body:MatchBody "}"
    MatchBody!
        case => case:Case body:Block next:MatchBody
        none => NONE
    Case!
        case => const:Const "|" next:Case
        end => const:Const
        default => "*"
    Const!
        const => c:Ident
        const => c:Ebased
        const => c:Efloat

    Struct!
        struct => name:Named "type" "{" body:StructBody "}"
        decl => name:Named "type"
    StructBody!
        field => field:Field z:Sep next:StructBody
        last => field:Field z:Sep

    Impl!
        impl => name:Named "impl" "{" body:ImplBody "}"
    ImplBody!
        method => meth:Method z:Sep next:ImplBody
        last => meth:Method z:Sep

    Enum!
        enum => name:Named "enum" "{" body:EnumBody "}"
    EnumBody!
        field => field:Name z:Sep next:EnumBody
        last => field:Name z:Sep

    Union!
        union => name:Named "union" "{" body:UnionBody "}"
        decl => name:Named "union"
    UnionBody!
        field => field:Field z:Sep next:UnionBody
        last => field:Field z:Sep

    Field! => name:Named type:Type
    Declaration! => name:Named type:ModType
    Definition!
        typed => decl:Declaration "=" value:Expr z:Sep
        infer => name:Named mod:Modifier "=" value:Expr z:Sep

    With!
        alias  => "with" type:ModType "as" alias:ModType
        extern => "with" extern:Named type:ModType
        clib   => "with" "c" ":"":" library:Library
        import => "with" library:Library
    Library!
        module => library:Name ":" ":" module:Library
        library => library:Name

    Function!
        deftp  => name:Named "(" params:Plist ")" "-"">" ret:Type body:Block
        decltp => name:Named "(" params:Plist ")" "-"">" ret:Type
        defp   => name:Named "(" params:Plist ")" body:Block
        declp  => name:Named "(" params:Plist ")"
        deft   => name:Named "-"">" ret:Type body:Block
        declt  => name:Named "-"">" ret:Type
        def    => name:Named body:Block
        decl   => name:Named "("")"
    Method!
        deftp  => name:Named "(" params:Plist ")" "-"">" ret:Type body:Block
        decltp => name:Named "(" params:Plist ")" "-"">" ret:Type
        defp   => name:Named "(" params:Plist ")" body:Block
        declp  => name:Named "(" params:Plist ")"
        deft   => name:Named "-"">" ret:Type body:Block
        declt  => name:Named "-"">" ret:Type
        def    => name:Named body:Block
        decl   => name:Named "("")"

    Return! => "return" expr:Expr

    Named!
        name => name:Name ":"

    Expr! => e:Eassign

    Eassign!
        assign => l:Eor e:REassign
    REassign!
        assign => "=" r:Eor n:REassign
        add    => "+""=" r:Eor n:REassign
        sub    => "-""=" r:Eor n:REassign
        mul    => "*""=" r:Eor n:REassign
        div    => "/""=" r:Eor n:REassign
        rsh    => ">"">""=" r:Eor n:REassign
        lsh    => "<""<""=" r:Eor n:REassign
        bxor   => "^""=" r:Eor n:REassign
        band   => "&""=" r:Eor n:REassign
        bor    => "|""=" r:Eor n:REassign
        bmod   => "%""=" r:Eor n:REassign
        none   => NONE

    Eor!
        or  => l:Eand e:REor
    REor!
        or   => "or" r:Eand n:REor
        none => NONE

    Eand!
        and => l:Ebor e:REand
    REand!
        and => "and" r:Ebor n:REand
        none => NONE

    Ebor!
        bor  => l:Ebxor e:REbor
    REbor!
        bor => "|" r:Ebxor n:REbor
        none => NONE

    Ebxor!
        bxor => l:Eband e:REbxor
    REbxor!
        bxor => "^" r:Eband n:REbxor
        none => NONE

    Eband!
        band => l:Eeq e:REband
    REband!
        band => "&" r:Eeq n:REband
        none => NONE

    Eeq!
        eq  => l:Ecmp e:REeq
    REeq!
        eq => "=" "=" r:Ecmp n:REeq
        ne => "!" "=" r:Ecmp n:REeq
        none => NONE

    Ecmp!
        cmp   => l:Eshift e:REcmp
    REcmp!
        le => "<" "=" r:Eshift n:REcmp
        ge => ">" "=" r:Eshift n:REcmp
        lt => "<" r:Eshift n:REcmp
        gt => ">" r:Eshift n:REcmp
        none => NONE

    Eshift!
        shift => l:Eadd e:REshift
    REshift!
        lshift => "<" "<" r:Eadd n:REshift
        rshift => ">" ">" r:Eadd n:REshift
        none => NONE

    Eadd!
        add => l:Emul e:REadd
    REadd!
        add => "+" r:Emul n:REadd
        sub => "-" r:Emul n:REadd
        none => NONE

    Emul!
        mul   => l:Eunary e:REmul
    REmul!
        mul => "*" r:Eunary n:REmul
        div => "/" r:Eunary n:REmul
        mod => "%" r:Eunary n:REmul
        none => NONE

    Eunary!
        self   => "'" r:Eaccess
        bnot   => "~" r:Eaccess
        not    => "not" r:Eaccess
        pos    => "+" r:Eaccess
        neg    => "-" r:Eaccess
        addr   => "@" mod:Modifier r:Eaccess
        cast   => l:Eaccess ":" r:ModType
        access => e:Eaccess

    Eaccess!
        deref  => "[" addr:Expr "]" e:REaccess
        access => l:Ident e:REaccess
        term   => e:Eterm
    REaccess!
        deref  => "[" addr:Expr "]" e:REaccess
        call   => "(" args:Elist ")" e:REaccess
        member => "." r:Ident n:REaccess
        derefm => "-"">" r:Ident n:REaccess
        none => NONE

    Eterm!
        t => t:Estring
        t => t:Echar
        t => t:Efloat
        t => t:Ebased
        t => t:Ident
        paren => "(" t:Expr ")"
        list => "{" t:Elist "}"
        block => "{" t:LocalProgram "}"

    Echar! => "'" char:REchar "'"
    REchar!
        ch => s:WORD
        ch => s:NUMBER
        esc => n:SYMBOL s:REchar
        none => NONE

    Ident!
        scope => scope:Name ":" ":" member:Ident
        ident => ident:Name

    Name!
        lname => name:WORD next:UName
        name => name:UName
        name => name:WORD
    UName!
        uline => "_" next:Name

    Estring! => s:STRING next:REstring
    REstring!
        part => s:STRING next:REstring
        end => NONE

    Ebased!
        hex => "0" "x" t:Ehex
        oct => "0" "o" t:Eint
        bin => "0" "b" t:Eint
        dec => t:Eint
    Efloat! => dec:Eint "." frac:Eint
    Eint!
        long => part:NUMBER "_" next:Eint
        part => part:NUMBER
    Ehex!
        hex => h:WORD "_" t:Ehex
        hex => h:NUMBER "_" t:Ehex
        end => t:WORD
        end => t:NUMBER

    Elist!
        item => l:Expr r:RElist
        none => NONE
    RElist!
        end  => "," r:Expr n:RElist
        none => ","
        none => NONE
    Tlist!
        type => type:ModType "," next:Tlist
        end  => type:ModType
    Plist!
        self => "self" ":" type:ModType.self "," next:Plist
        parm => name:Named type:ModType "," next:Plist
        ends => "self" ":" type:ModType.self
        end => name:Named type:ModType

    ModType!
        type  => mod:Modifier t:Name
        fptrpt=> mod:Modifier "[""(" params:Tlist ")" "-"">" rtype:Type "]"
        fptrt => mod:Modifier "[""("")" "-"">" rtype:Type "]"
        fptrt => mod:Modifier "[""-"">" rtype:Type "]"
        fptrp => mod:Modifier "[""(" params:Tlist ")""]"
        array => mod:Modifier "[" type:ModType ":" size:Ebased "]"
        ptr   => mod:Modifier "[" type:ModType "]"
        voidp => mod:Modifier "[""]"
        void  => "("")"
        self  => mod:Modifier self:"Self"
    Type!
        type  => t:Name
        fptrpt=> "[""(" params:Tlist ")" "-"">" rtype:Type "]"
        fptrt => "[""("")" "-"">" rtype:Type "]"
        fptrt => "[""-"">" rtype:Type "]"
        fptrp => "[""(" params:Tlist ")""]"
        array => "[" type:ModType ":" size:Ebased "]"
        ptr   => "[" type:ModType "]"
        voidp => "[""]"
        void  => "("")"
        self  => self:"Self"

    Modifier!
        mutstatic => "mut" "static"
        mutstatic => "static" "mut"
        mutable => "mut"
        static => "static"
        const => NONE



:: prefixes: down
    Enum
        enum  => (set body prefix name)
    EnumBody
        field => (set next prefix prefix)

    Impl
        impl =>
            (set body prefix name)

    ImplBody
        method =>
            (set meth prefix prefix)
            (set next prefix prefix)
        last =>
            (set meth prefix prefix)

    Method
        deft  =>
            (set ret self prefix)
        defp  =>
            (set params self prefix)
        deftp =>
            (set ret self prefix)
            (set params self prefix)
    Plist
        self => (set type self self)
        ends => (set type self self)



:: exprprep
    Eassign
        assign => (set e l l)

    REassign
        assign => (set n l r)
        add    => (set n l r)
        sub    => (set n l r)
        mul    => (set n l r)
        div    => (set n l r)
        rsh    => (set n l r)
        lsh    => (set n l r)
        bxor   => (set n l r)
        band   => (set n l r)
        bor    => (set n l r)
        bmod   => (set n l r)

    Eor
        or  => (set e l l)
    REor
        or => (set n l r)

    Eand
        and => (set e l l)
    REand
        and => (set n l r)

    Ebor
        bor => (set e l l)
    REbor
        bor => (set n l r)

    Ebxor
        bxor => (set e l l)
    REbxor
        bxor => (set n l r)

    Eband
        band => (set e l l)
    REband
        band => (set n l r)

    Eeq
        eq => (set e l l)
    REeq
        eq => (set n l r)
        ne => (set n l r)

    Ecmp
        cmp => (set e l l)
    REcmp
        le => (set n l r)
        ge => (set n l r)
        lt => (set n l r)
        gt => (set n l r)

    Eshift
        shift => (set e l l)
    REshift
        lshift => (set n l r)
        rshift => (set n l r)

    Eadd
        add => (set e l l)
    REadd
        add => (set n l r)
        sub => (set n l r)

    Emul
        mul => (set e l l)
    REmul
        mul => (set n l r)
        div => (set n l r)
        mod => (set n l r)



:: output
    Program
        stmt => s / next
        end => s
    LocalProgram
        stmt => s / next
        end => s

    Block
        body => "{" > body < "}"
        empty => "{}"

    GlobalStatement
        p => s
        s => s ";"

    LocalStatement
        p => s
        s => s ";"

    Macro => "#" macro

    Loop
        named => "while (1)" / "skip_"name":" / body / "break_"name":"
        anon => "while (1)" / body
    Break
        named => "goto break_" name
        anon => "break"
    Skip
        named => "goto skip_" name
        anon => "continue"

    If => "if (" cond ")" / body other
    Else
        else => / "else" / body

    Match => "switch (" cond ")" / "{" >body< "}"
    MatchBody
        case => case / body / "break;" / / next
    Case
        case => "case " const ":" / next
        end => "case " const ":"
        default => "default:"
    Const
        const => c

    Struct
        struct => /"typedef struct "name /"{" >body< "}" /name
        decl => /"typedef struct " name " " name
    StructBody
        field => field ";"/ next
        last => field ";"

    Impl
        impl => / "#define Self " name body / "#undef Self"

    ImplBody
        method => meth / next
        last => meth

    Enum
        enum => / "typedef enum " name / "{" > body < "}" / name
    EnumBody
        field => prefix "_" field "," / next
        last => prefix "_" field

    Union
        union => / "typedef union " name / "{" > body < "}" / name
        decl => / "typedef union " name " " name
    UnionBody
        field => field ";" / next
        last => field

    Declaration => type name
    Field => type name
    Definition
        typed => decl " = " value
        infer => mod "typeof("value") " name " = " value


    With
        clib => "#include <" library ".h>"
        import => "#include <" library ".c>"
        extern => "extern " type extern
        alias  => "typedef " type alias
    Library
        module => library "/" module
        library => library

    Function
        def   => / "void " name " (void)" / body
        defp  => / "void " name " (" params ")" / body
        deft  => / ret name " (void)" / body
        deftp => / ret name " (" params ")" / body
        decl   => / "void " name " (void)"
        declp  => / "void " name " (" params ")"
        declt  => / ret name " (void)"
        decltp => / ret name " (" params ")"
    Method
        def   => / "void " prefix"_"name " (void)" / body
        defp  => / "void " prefix"_"name " (" params ")" / body
        deft  => / ret prefix"_"name " (void)" / body
        deftp => / ret prefix"_"name " (" params ")" / body
        decl   => / "void " prefix"_"name " (void)"
        declp  => / "void " prefix"_"name " (" params ")"
        declt  => / ret prefix"_"name " (void)"
        decltp => / ret prefix"_"name " (" params ")"
    Return => "return " expr

    Named
        name => name

    Expr => e

    Eassign
        assign => e
    REassign
        assign => l " = " n
        add    => l " += " n
        sub    => l " -= " n
        mul    => l " *= " n
        div    => l " /= " n
        rsh    => l " >>= " n
        lsh    => l " <<= " n
        bxor   => l " ^= " n
        band   => l " &= " n
        bor    => l " |= " n
        bmod   => l " %= " n
        none   => l

    Eor
        or  => e
    REor
        or   => "or(" l  ", " n ")"
        none => l

    Eand
        and => e
    REand
        and  => "and(" l ", " n ")"
        none => l

    Ebor
        bor  => e
    REbor
        bor  => l " | " n
        none => l

    Ebxor
        bxor => e
    REbxor
        bxor => l " ^ " n
        none => l

    Eband
        band => e
    REband
        band => l " & " n
        none => l

    Eeq
        eq  => e
    REeq
        eq   => l " == " n
        ne   => l " != " n
        none => l

    Ecmp
        cmp   => e
    REcmp
        le   => l " <= " n
        ge   => l " >= " n
        lt   => l " < " n
        gt   => l " > " n
        none => l

    Eshift
        shift => e
    REshift
        lshift => l " << " n
        rshift => l " >> " n
        none   => l

    Eadd
        add => e
    REadd
        add  => l " + " n
        sub  => l " - " n
        none => l

    Emul
        mul   => e
    REmul
        mul  => l " * " n
        div  => l " / " n
        mod  => l " % " n
        none => l

    Eunary
        self   => "self." r
        bnot   =>  "~" r
        not    =>  "!" r
        pos    =>  "+" r
        neg    =>  "-" r
        addr   =>  "(typeof("r") "mod"*)" "&" r
        cast   => "(" r ")" l
        access => e

    Eaccess
        deref  => "*(" addr ")" e
        access => l e
        term   => e
    REaccess
        deref  => "[" addr "]" e
        call   => "(" args ")" e
        member => "." r n
        derefm => "-"">" r n

    Eterm
        t => t
        paren => "(" t ")"
        list => "{" t "}"
        block => "({" >t< "})"

    Echar => "'" char "'"
    REchar
        ch => s
        esc => n s

    Ident
        scope => scope "_" member
        ident => ident
    Name
        lname => name next
        name => name
    UName
        uline => "_" next

    Estring => '"' s next '"'
    REstring
        part => s next

    Ebased
        hex => "0x" t
        oct => "0o" t
        bin => "0b" t
        dec => t
    Efloat => dec "." frac
    Eint
        long => part next
        part => part
    Ehex
        hex => h t
        end => t

    Elist
        item => l r

    RElist
        end  => ", " r n

    Tlist
        type => type ", " next
        end  => type
    Plist
        self => type "self, " next
        parm => type name ", " next
        ends => type "self"
        end => type name

    ModType
        self  => mod self " "
        type  => mod t " "
        fptrpt=> mod "typeof(" rtype "(*) (" params ")) "
        fptrt => mod "typeof(" rtype "(*) (void)) "
        fptrp => mod "typeof(void (*) (" params ")) "
        array => mod "typeof(" type "[" size "]" ") "
        ptr   => type "*" mod
        voidp => "void *" mod
        void  => "void "
    Type
        self  => self " "
        type  => t " "
        fptrpt=> "typeof(" rtype "(*) (" params ")) "
        fptrt => "typeof(" rtype "(*) (void)) "
        fptrp => "typeof(void (*) (" params ")) "
        array => "typeof(" type "[" size "]" ") "
        ptr   => type "*"
        voidp => "void *"
        void  => "void "
    Modifier
        mutstatic => "static "
        static => "static const "
        const => "const "

