# Minishell Phase 1: Core Input / Startup Documentation
## توثيق المرحلة الأولى من Minishell: الإدخال الأساسي / بدء التشغيل

---

## Table of Contents / فهرس المحتويات
1. [Overview / نظرة عامة](#overview)
2. [File Structure / هيكل الملفات](#file-structure)
3. [Execution Flow / تدفق التنفيذ](#execution-flow)
4. [File-by-File Analysis / تحليل ملف بملف](#file-by-file-analysis)
5. [Signal Handling / التعامل مع الإشارات](#signal-handling)
6. [Memory Management / إدارة الذاكرة](#memory-management)
7. [TTY vs Non-TTY Behavior / سلوك TTY مقابل Non-TTY](#tty-vs-non-tty-behavior)

---

## Overview / نظرة عامة

المرحلة الأولى من minishell مسؤولة عن:
- **بدء التشغيل الأولي** (Initialization)
- **قراءة الإدخال** (Input Reading)
- **إدارة الإشارات** (Signal Management)
- **إعداد البيئة** (Environment Setup)
- **معالجة الاقتباسات المفتوحة** (Open Quotes Handling)

---

## File Structure / هيكل الملفات

### Core Files / الملفات الأساسية:
```
src/core/
├── main.c                 # نقطة البداية الرئيسية
├── input.c               # قراءة الإدخال الأساسية
├── input_helpers.c       # مساعدات قراءة الإدخال
├── signals.c             # إدارة الإشارات
├── core_utils.c          # أدوات أساسية
├── core_state.c          # إدارة الحالة الأساسية
├── core_state2.c         # إدارة الحالة (جزء 2)
├── core_state3.c         # إدارة الحالة (جزء 3)
├── cleanup.c             # تنظيف الذاكرة
├── env_utils.c           # أدوات البيئة
├── env_utils2.c          # أدوات البيئة (جزء 2)
├── env_helpers.c         # مساعدات البيئة
└── globals.c             # المتغيرات العامة

include/
└── mandatory.h           # ملف الرؤوس الرئيسي
```

---

## Execution Flow / تدفق التنفيذ

### 1. Program Entry Point / نقطة دخول البرنامج
```c
int main(int argc, char **argv, char **envp)
```

### 2. Bootstrap Process / عملية التهيئة
```c
envp = bootstrap_env(argv, envp, &node);
```

### 3. Main Loop / الحلقة الرئيسية
```c
while (1)
    envp = main_loop(envp, &node);
```

---

## File-by-File Analysis / تحليل ملف بملف

---

## 1. main.c - نقطة البداية الرئيسية

### الوظيفة الرئيسية:
```c
int main(int argc, char **argv, char **envp)
{
    t_node node;
    
    (void)argc;
    ft_bzero(&node, sizeof(t_node));
    envp = bootstrap_env(argv, envp, &node);
    while (1)
        envp = main_loop(envp, &node);
}
```

### الدوال المساعدة:

#### `bootstrap_env()` - تهيئة البيئة
```c
static char **bootstrap_env(char **argv, char **envp, t_node *node)
{
    init_node(node);                    // تهيئة العقدة
    set_exit_status(0);                 // تعيين حالة الخروج
    envp = strarrdup(envp);             // نسخ متغيرات البيئة
    envp = shlvl_plus_plus(setpwd(node, envp)); // تحديث SHLVL
    envp = ft_setenv_envp("OLDPWD", NULL, envp); // تعيين OLDPWD
    envp = ft_setenv_envp("_", argv[0], envp);   // تعيين مسار البرنامج
    node->path_fallback = NULL;         // تهيئة مسار الاحتياط
    node->line_nbr = 0;                 // تهيئة رقم السطر
    set_signal();                       // إعداد الإشارات
    return (envp);
}
```

#### `main_loop()` - الحلقة الرئيسية
```c
static char **main_loop(char **envp, t_node *n)
{
    char *line;
    char *prompt;
    
    handle_signals();                   // معالجة الإشارات
    prompt = get_and_process_prompt(envp, n); // الحصول على المطالبة
    set_interactive_mode(true);         // تفعيل الوضع التفاعلي
    line = get_line(prompt);            // قراءة السطر
    set_interactive_mode(false);        // إلغاء الوضع التفاعلي
    if (prompt)
        free(prompt);                   // تحرير المطالبة
    n->line_nbr++;                      // زيادة رقم السطر
    if (!line)
        handle_eof_exit(envp, n);       // معالجة نهاية الملف
    envp = process_command(line, envp, n); // معالجة الأمر
    if (isatty(STDIN_FILENO))
        clear_history();                // مسح التاريخ
    return (envp);
}
```

#### `get_and_process_prompt()` - الحصول على المطالبة
```c
static char *get_and_process_prompt(char **envp, t_node *n)
{
    const char *prompt;
    char *prompt_copy;
    
    prompt = ft_getenv("PS1", envp);    // الحصول على PS1
    if (!prompt)
        prompt = "minishell🍭$ ";       // المطالبة الافتراضية
    prompt_copy = ft_strdup(prompt);    // نسخ المطالبة
    return (expand_prompt(prompt_copy, envp, n)); // توسيع المطالبة
}
```

#### `handle_signals()` - معالجة الإشارات
```c
static void handle_signals(void)
{
    if (get_signal_number() == SIGINT)
    {
        set_exit_status(130);           // تعيين حالة الخروج للإشارة
        clear_signal_number();          // مسح رقم الإشارة
    }
}
```

---

## 2. input.c - قراءة الإدخال الأساسية

### الدوال الرئيسية:

#### `get_line()` - الحصول على السطر
```c
char *get_line(char *str)
{
    char *line;
    char *prompt;
    
    if (!isatty(STDIN_FILENO))          // إذا لم يكن TTY
        return (read_line_non_tty());   // قراءة غير تفاعلية
    prompt = ft_strdup(str);            // نسخ المطالبة
    if (!prompt)
        return (NULL);
    line = get_continuation_line(prompt); // الحصول على سطر متعدد
    if (line && !is_blank(line))        // إذا لم يكن فارغاً
        add_history(line);              // إضافة للتاريخ
    free(prompt);                       // تحرير المطالبة
    return (line);
}
```

#### `read_line_non_tty()` - قراءة غير تفاعلية
```c
char *read_line_non_tty(void)
{
    char *buf;
    size_t cap;
    ssize_t nread;
    
    buf = NULL;
    cap = 0;
    nread = getline(&buf, &cap, stdin); // قراءة باستخدام getline
    if (nread < 0)
    {
        free(buf);
        return (NULL);
    }
    mark_nontext(buf, (size_t)nread);   // تحديد النص غير العادي
    if (nread > 0 && buf[nread - 1] == '\n')
        buf[nread - 1] = '\0';          // إزالة السطر الجديد
    return (buf);
}
```

#### `handle_eof_exit()` - معالجة نهاية الملف
```c
void handle_eof_exit(char **envp, t_node *node)
{
    if (node)
    {
        free(node->pwd);                // تحرير مسار العمل
        free(node->path_fallback);      // تحرير مسار الاحتياط
    }
    if (envp)
        strarrfree(envp);               // تحرير متغيرات البيئة
    clear_history();                    // مسح التاريخ
    restore_termios();                  // استعادة إعدادات الطرفية
    maybe_write_exit_file();            // كتابة ملف الخروج
    exit(get_exit_status());            // الخروج مع الحالة
}
```

#### `process_read_line()` - معالجة قراءة السطر
```c
int process_read_line(char **result, char **cur_prompt, char *orig)
{
    char *line;
    char *tmp_prompt;
    
    line = NULL;
    tmp_prompt = NULL;
    line = readline(*cur_prompt);       // قراءة باستخدام readline
    if (!line)
    {
        free(*result);
        return (-1);
    }
    if (!append_line(result, line))     // إضافة السطر
        return (-1);
    if (quote_check(*result, (int)ft_strlen(*result), NULL) == 0)
        return (0);                     // إذا كانت الاقتباسات مكتملة
    if (*cur_prompt != orig)
        free(*cur_prompt);              // تحرير المطالبة السابقة
    tmp_prompt = ft_strdup("> ");       // مطالبة الاستمرار
    if (!tmp_prompt)
        return (-1);
    *cur_prompt = tmp_prompt;
    return (1);                         // يحتاج استمرار
}
```

#### `mark_nontext()` - تحديد النص غير العادي
```c
static void mark_nontext(const char *buf, size_t nread)
{
    size_t i;
    unsigned char c;
    
    i = 0;
    while (i < nread)
    {
        c = (unsigned char)buf[i];
        if (c != '\n' && ((c < 32 && c != '\t') || c >= 128))
        {
            set_nontext_input(true);    // تحديد وجود نص غير عادي
            break;
        }
        i++;
    }
}
```

---

## 3. input_helpers.c - مساعدات قراءة الإدخال

### الدوال المساعدة:

#### `get_continuation_line()` - الحصول على سطر متعدد
```c
char *get_continuation_line(char *prompt)
{
    char *result;
    char *current_prompt;
    int st;
    
    if (!isatty(STDIN_FILENO))          // إذا لم يكن TTY
        return (readline(NULL));        // قراءة مباشرة
    result = NULL;
    current_prompt = prompt;
    while (1)
    {
        st = process_read_line(&result, &current_prompt, prompt);
        if (st < 0)                     // خطأ
        {
            if (current_prompt != prompt)
                free(current_prompt);
            result = NULL;
            return (NULL);
        }
        if (st == 0)                    // اكتمال الاقتباسات
        {
            if (current_prompt != prompt)
                free(current_prompt);
            return (result);
        }
        // st == 1: يحتاج استمرار
    }
}
```

#### `append_line()` - إضافة سطر
```c
int append_line(char **result, char *line)
{
    char *tmp;
    char *joined;
    
    tmp = NULL;
    joined = NULL;
    if (*result)                        // إذا كان هناك نتيجة سابقة
    {
        tmp = ft_strjoin(*result, "\n"); // ربط مع سطر جديد
        if (!tmp)
            return (free(line), 0);
        free(*result);
        joined = ft_strjoin(tmp, line); // ربط مع السطر الجديد
        free(tmp);
        if (!joined)
            return (free(line), 0);
        *result = joined;
    }
    else
    {
        *result = ft_strdup(line);      // نسخ السطر
        if (!*result)
            return (free(line), 0);
    }
    free(line);                         // تحرير السطر الأصلي
    return (1);
}
```

---

## 4. signals.c - إدارة الإشارات

### الدوال الرئيسية:

#### `set_signal()` - إعداد الإشارات
```c
void set_signal(void)
{
    if (isatty(STDIN_FILENO))          // إذا كان TTY
        set_termios();                  // إعداد إعدادات الطرفية
    signal(SIGINT, sigint_handler);     // إعداد معالج Ctrl+C
    signal(SIGQUIT, SIG_IGN);           // تجاهل Ctrl+\
    signal(SIGPIPE, SIG_IGN);           // تجاهل كسر الأنبوب
}
```

#### `sigint_handler()` - معالج Ctrl+C
```c
static void sigint_handler(int sig)
{
    (void)sig;
    set_signal_number(SIGINT);          // تعيين رقم الإشارة
    if (is_interactive_mode())          // إذا كان في الوضع التفاعلي
    {
        write(STDOUT_FILENO, "\n", 1);  // طباعة سطر جديد
        rl_on_new_line();               // إعلام readline بسطر جديد
        rl_replace_line("", 0);         // استبدال السطر الحالي
        rl_redisplay();                 // إعادة عرض المطالبة
    }
}
```

#### `set_heredoc_signal()` - إعداد إشارات heredoc
```c
void set_heredoc_signal(void)
{
    struct sigaction sa;
    
    ft_bzero(&sa, sizeof(sa));
    sa.sa_handler = sigint_handler_heredoc; // معالج مخصص لـ heredoc
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);       // استخدام sigaction بدلاً من signal
    signal(SIGQUIT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
}
```

---

## 5. core_utils.c - الأدوات الأساسية

### الدوال المساعدة:

#### `is_blank()` - فحص السطر الفارغ
```c
bool is_blank(const char *s)
{
    int i;
    
    i = 0;
    while (s && s[i])
    {
        if (s[i] != ' ' && s[i] != '\t' && s[i] != '\r'
            && s[i] != '\v' && s[i] != '\f')
            return (false);             // ليس فارغاً
        i++;
    }
    return (true);                      // فارغ
}
```

#### `ft_atoll()` - تحويل نص إلى رقم
```c
long long ft_atoll(const char *str)
{
    size_t idx;
    int sign;
    long long result;
    
    if (!str)
        return (0);
    idx = 0;
    result = 0;
    sign = 1;
    while (1)
    {
        if ((str[idx] > '\b' && str[idx] <= '\r') || str[idx] == ' ')
            idx++;                      // تخطي المسافات البيضاء
        else
            break;
    }
    if (str[idx] == '-')
        sign = -1;                      // تحديد الإشارة
    idx += (str[idx] == '+' || str[idx] == '-');
    while (str[idx] >= '0' && str[idx] <= '9')
        result = result * 10 + (str[idx++] - '0'); // تحويل الأرقام
    result *= sign;
    return (result);
}
```

#### `set_termios()` - إعداد إعدادات الطرفية
```c
void set_termios(void)
{
    struct termios termios;
    struct termios original;
    
    if (!isatty(STDIN_FILENO))          // إذا لم يكن TTY
        return;
    if (tcgetattr(STDIN_FILENO, &original) == 0)
    {
        termios = original;
        termios.c_lflag &= ~(tcflag_t)ECHOCTL; // إلغاء عرض أحرف التحكم
        tcsetattr(STDIN_FILENO, TCSANOW, &termios);
    }
}
```

#### `restore_termios()` - استعادة إعدادات الطرفية
```c
void restore_termios(void)
{
    struct termios original;
    
    if (!isatty(STDIN_FILENO))          // إذا لم يكن TTY
        return;
    if (tcgetattr(STDIN_FILENO, &original) == 0)
    {
        original.c_lflag |= (tcflag_t)ECHOCTL; // إعادة تفعيل عرض أحرف التحكم
        tcsetattr(STDIN_FILENO, TCSANOW, &original);
    }
}
```

---

## 6. core_state.c - إدارة الحالة الأساسية

### الدوال الرئيسية:

#### `ms_slots()` - الحصول على فتحات الحالة العامة
```c
struct s_global_slots *ms_slots(void)
{
    static struct s_global_slots state = {0, 0, 0};
    return (&state);
}
```

#### `get_signal_number()` - الحصول على رقم الإشارة
```c
int get_signal_number(void)
{
    return ((int)ms_slots()->signal_number);
}
```

#### `clear_signal_number()` - مسح رقم الإشارة
```c
void clear_signal_number(void)
{
    ms_slots()->signal_number = 0;
}
```

#### `set_signal_number()` - تعيين رقم الإشارة
```c
void set_signal_number(int sig)
{
    ms_slots()->signal_number = (sig_atomic_t)sig;
}
```

#### `is_interactive_mode()` - فحص الوضع التفاعلي
```c
bool is_interactive_mode(void)
{
    return (ms_slots()->interactive != 0);
}
```

---

## 7. core_state2.c - إدارة الحالة (جزء 2)

### الدوال الرئيسية:

#### `set_interactive_mode()` - تعيين الوضع التفاعلي
```c
void set_interactive_mode(bool value)
{
    ms_slots()->interactive = value;
}
```

#### `_ms_exit_status()` - إدارة حالة الخروج الداخلية
```c
int _ms_exit_status(int op, int value)
{
    if (op)
        ms_slots()->exit_status = value;
    return (ms_slots()->exit_status);
}
```

#### `get_exit_status()` - الحصول على حالة الخروج
```c
int get_exit_status(void)
{
    return (_ms_exit_status(0, 0));
}
```

#### `set_exit_status()` - تعيين حالة الخروج
```c
void set_exit_status(int status)
{
    (void)_ms_exit_status(1, status);
}
```

---

## 8. core_state3.c - إدارة الحالة (جزء 3)

### الدوال الرئيسية:

#### `get_nontext_input()` - فحص الإدخال غير النصي
```c
bool get_nontext_input(void)
{
    return (g_nontext_input != 0);
}
```

#### `set_nontext_input()` - تعيين الإدخال غير النصي
```c
void set_nontext_input(bool v)
{
    g_nontext_input = (v != 0);
}
```

#### `clear_nontext_input()` - مسح الإدخال غير النصي
```c
void clear_nontext_input(void)
{
    g_nontext_input = 0;
}
```

#### `maybe_write_exit_file()` - كتابة ملف الخروج
```c
void maybe_write_exit_file(void)
{
    const char *path;
    int fd;
    char *s;
    
    path = getenv("MINISHELL_EXIT_FILE");
    if (!path || !*path)
        return;                         // لا يوجد ملف خروج مطلوب
    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        return;                         // فشل في فتح الملف
    s = ft_itoa(get_exit_status());
    if (s)
    {
        write(fd, s, ft_strlen(s));     // كتابة حالة الخروج
        free(s);
    }
    close(fd);
}
```

---

## 9. cleanup.c - تنظيف الذاكرة

### الدوال الرئيسية:

#### `cleanup_env()` - تنظيف البيئة
```c
void cleanup_env(char **envp, t_node *node)
{
    if (envp)
        strarrfree(envp);               // تحرير متغيرات البيئة
    if (node && node->pwd)
        free(node->pwd);                // تحرير مسار العمل
    if (node && node->path_fallback)
        free(node->path_fallback);      // تحرير مسار الاحتياط
    if (node && node->ori_args)
    {
        strarrfree(node->ori_args);     // تحرير الوسائط الأصلية
        node->ori_args = NULL;
    }
    clear_history();                    // مسح التاريخ
    restore_termios();                  // استعادة إعدادات الطرفية
    set_exit_status(0);                 // تعيين حالة الخروج
    if (node)
    {
        node->pwd = NULL;
        node->path_fallback = NULL;
        node->ori_args = NULL;
    }
    envp = NULL;
}
```

---

## 10. env_utils.c - أدوات البيئة

### الدوال الرئيسية:

#### `setpwd()` - تعيين مسار العمل
```c
char **setpwd(t_node *node, char **envp)
{
    char *curdir[2];
    char *pwd_env;
    
    curdir[0] = getcwd(0, 0);           // الحصول على المسار الحالي
    pwd_env = ft_getenv("PWD", envp);
    if (pwd_env)
        node->pwd = ft_strdup(pwd_env); // نسخ PWD من البيئة
    else
        node->pwd = NULL;
    if (!node->pwd || chdir(node->pwd) == -1)
        return (handle_pwd_error(node, envp, curdir[0]));
    curdir[1] = getcwd(0, 0);           // الحصول على المسار بعد التغيير
    return (handle_pwd_change(node, envp, curdir));
}
```

#### `shlvl_mod()` - تعديل مستوى القشرة
```c
char **shlvl_mod(int mod, char **envp)
{
    int newval;
    char *tmp;
    char *shlvl_env;
    
    shlvl_env = ft_getenv("SHLVL", envp);
    if (shlvl_env)
        newval = ft_atoi(shlvl_env) + mod; // زيادة أو تقليل المستوى
    else
        newval = 1 + mod;
    if (newval > 1000)
        newval = 1;                     // إعادة تعيين إذا كان عالياً جداً
    if (newval < 0)
        newval = 0;                     // لا يمكن أن يكون سالباً
    tmp = ft_itoa(newval);
    envp = ft_setenv_envp("SHLVL", tmp, envp);
    free(tmp);
    return (envp);
}
```

#### `ft_getenv()` - الحصول على متغير البيئة
```c
char *ft_getenv(const char *name, char **envp)
{
    int i;
    size_t len;
    
    i = 0;
    len = ft_strlen(name);
    while (envp[i] && (ft_strncmp(envp[i], name, len)
            || (envp[i][len] != '=' && envp[i][len] != '\0')))
        i++;
    if (!envp[i] || !ft_strchr(envp[i], '='))
        return (NULL);
    return (envp[i] + len + 1);         // إرجاع القيمة بعد =
}
```

---

## 11. env_utils2.c - أدوات البيئة (جزء 2)

### الدوال الرئيسية:

#### `shlvl_plus_plus()` - زيادة مستوى القشرة
```c
char **shlvl_plus_plus(char **envp)
{
    char *str;
    char *new_str;
    
    str = ft_getenv("SHLVL", envp);
    new_str = determine_new_shlvl(str);
    if (!new_str)
    {
        strarrfree(envp);
        exit(EXIT_FAILURE);
    }
    envp = ft_setenv_envp("SHLVL", new_str, envp);
    free(new_str);
    return (envp);
}
```

#### `determine_new_shlvl()` - تحديد مستوى القشرة الجديد
```c
static char *determine_new_shlvl(char *str)
{
    int current_level;
    int new_level;
    
    if (!str || !ft_isalldigit(str))
        return (ft_itoa(1));            // مستوى افتراضي
    current_level = ft_atoi(str);
    new_level = current_level + 1;
    if (new_level >= 1000)
    {
        handle_shell_level_warning(new_level);
        return (ft_itoa(1));            // إعادة تعيين
    }
    if (new_level <= 0)
        return (ft_itoa(0));            // لا يمكن أن يكون سالباً
    return (ft_itoa(new_level));
}
```

---

## 12. env_helpers.c - مساعدات البيئة

### الدوال الرئيسية:

#### `ft_setenv_envp()` - تعيين متغير البيئة
```c
char **ft_setenv_envp(const char *name, const char *value, char **envp)
{
    int i;
    char *str;
    
    if (!name || !*name || ft_strchr(name, '='))
        return (envp);                  // اسم غير صالح
    str = build_env_pair_for_envp(name, value);
    if (!str)
        exit(EXIT_FAILURE);
    i = find_env_index_local(name, envp);
    if (envp[i])
    {
        free(envp[i]);                  // تحرير القيمة القديمة
        envp[i] = str;                  // تعيين القيمة الجديدة
    }
    else
        envp = strarradd_take(envp, str); // إضافة متغير جديد
    return (envp);
}
```

#### `ensure_oldpwd_export()` - ضمان وجود OLDPWD
```c
char **ensure_oldpwd_export(char **envp)
{
    if (!env_has_key_any(envp, "OLDPWD"))
        envp = ft_setenv_envp("OLDPWD", NULL, envp);
    return (envp);
}
```

---

## 13. globals.c - المتغيرات العامة

### المتغيرات العامة:
```c
int g_nontext_input = 0;               // متغير عام لتتبع الإدخال غير النصي
```

---

## Signal Handling / التعامل مع الإشارات

### 1. Ctrl+C (SIGINT)
- **في الوضع التفاعلي**: يطبع سطر جديد ويعيد عرض المطالبة
- **في heredoc**: يطبع سطر جديد فقط
- **يتم تعيين حالة الخروج إلى 130**

### 2. Ctrl+\ (SIGQUIT)
- **يتم تجاهله** في جميع الحالات

### 3. SIGPIPE
- **يتم تجاهله** في الوضع العادي
- **يتم استخدام السلوك الافتراضي** في العمليات الفرعية

---

## Memory Management / إدارة الذاكرة

### نقاط التحرير المهمة:

#### 1. في `main_loop()`:
```c
if (prompt)
    free(prompt);                       // تحرير المطالبة
```

#### 2. في `get_line()`:
```c
free(prompt);                           // تحرير المطالبة
```

#### 3. في `append_line()`:
```c
free(line);                             // تحرير السطر الأصلي
```

#### 4. في `handle_eof_exit()`:
```c
free(node->pwd);                        // تحرير مسار العمل
free(node->path_fallback);              // تحرير مسار الاحتياط
strarrfree(envp);                       // تحرير متغيرات البيئة
```

#### 5. في `cleanup_env()`:
```c
strarrfree(envp);                       // تحرير متغيرات البيئة
free(node->pwd);                        // تحرير مسار العمل
free(node->path_fallback);              // تحرير مسار الاحتياط
strarrfree(node->ori_args);             // تحرير الوسائط الأصلية
```

---

## TTY vs Non-TTY Behavior / سلوك TTY مقابل Non-TTY

### TTY (Terminal):
- **يستخدم readline** للقراءة التفاعلية
- **يدعم التاريخ** (history)
- **يدعم الإشارات** التفاعلية
- **يدعم الاقتباسات المفتوحة** مع مطالبة الاستمرار

### Non-TTY (Pipe/File):
- **يستخدم getline** للقراءة المباشرة
- **لا يدعم التاريخ**
- **لا يدعم الإشارات** التفاعلية
- **يقرأ حتى نهاية الملف** أو EOF

---

## Key Features / الميزات الرئيسية

### 1. Open Quotes Handling / معالجة الاقتباسات المفتوحة
- **يتم فحص الاقتباسات** في كل سطر
- **إذا كانت غير مكتملة**، يتم طلب استمرار
- **يتم ربط الأسطر** مع بعضها البعض

### 2. Signal Management / إدارة الإشارات
- **معالجة منفصلة** للوضع التفاعلي و heredoc
- **حفظ حالة الإشارات** في متغيرات عامة
- **استعادة الإعدادات** عند الخروج

### 3. Environment Management / إدارة البيئة
- **نسخ متغيرات البيئة** عند البدء
- **تحديث SHLVL** تلقائياً
- **إدارة PWD و OLDPWD**

### 4. Memory Safety / أمان الذاكرة
- **تحرير شامل** لجميع الذاكرة المخصصة
- **فحص NULL** قبل التحرير
- **استخدام strarrfree** للمصفوفات

---

## Conclusion / الخلاصة

المرحلة الأولى من minishell توفر:
- **نظام إدخال قوي** يدعم TTY و Non-TTY
- **معالجة شاملة للإشارات** مع دعم التفاعل
- **إدارة آمنة للذاكرة** مع تحرير شامل
- **دعم الاقتباسات المفتوحة** مع مطالبة الاستمرار
- **إدارة متقدمة للبيئة** مع تحديث تلقائي

هذا التصميم يضمن **استقرار** و **موثوقية** البرنامج في جميع الحالات الاستخدام المختلفة.
