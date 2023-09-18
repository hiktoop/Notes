# 关于yaml

## | 与 > 的区别

If you want to write a string that spans across multiple lines and you want to preserve the line breaks, use the pipe symbol (`|`):

```yaml
|
 I am message that spans multiple lines
 I go on and on across lines
 and lines
 and more lines
```

Make sure that the message is indented!

Alternatively, if you have a string in a YAML file that spans across 
multiple lines for readability, but you want the parser to interpret it 
as a single line string, you can use the `>` character, which will replace each line break with a space:

```yaml
>
 I am message that spans
 multiple lines
 but I will be parsed
 on one line
```
