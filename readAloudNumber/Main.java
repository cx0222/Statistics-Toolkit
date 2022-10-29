import com.formdev.flatlaf.FlatIntelliJLaf;

import javax.swing.*;
import java.awt.*;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.StringSelection;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.HashMap;
import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

// 陈  玄 2020311462
class InvalidInputException extends Exception {
    public InvalidInputException() {
        super();
    }

    @Override
    public String getMessage() {
        return "错误！输入的格式有误";
    }

    @Override
    public StackTraceElement[] getStackTrace() {
        return super.getStackTrace();
    }
}

class OutOfRangeException extends Exception {
    public OutOfRangeException() {
        super();
    }

    @Override
    public String getMessage() {
        return "错误！输入的数额超出范围";
    }

    @Override
    public StackTraceElement[] getStackTrace() {
        return super.getStackTrace();
    }
}

class Core {
    private static final Character YUAN = '元';
    private static final Character WAN = '万';
    private static final Character YI = '亿';
    private static final Character ZHENG = '整';
    private static final HashMap<Integer, String> CHINESE_CHARS;

    static {
        CHINESE_CHARS = new HashMap<>();
        // 壹、贰、叁、肆、伍、陆、柒、捌、玖、拾、佰、仟、万、亿、元、角、分、零、整
        CHINESE_CHARS.put(-10, "角");
        CHINESE_CHARS.put(-11, "分");
        CHINESE_CHARS.put(-12, "厘");
        CHINESE_CHARS.put(0, "零");
        CHINESE_CHARS.put(1, "壹");
        CHINESE_CHARS.put(2, "贰");
        CHINESE_CHARS.put(3, "叁");
        CHINESE_CHARS.put(4, "肆");
        CHINESE_CHARS.put(5, "伍");
        CHINESE_CHARS.put(6, "陆");
        CHINESE_CHARS.put(7, "柒");
        CHINESE_CHARS.put(8, "捌");
        CHINESE_CHARS.put(9, "玖");
        CHINESE_CHARS.put(100, "仟");
        CHINESE_CHARS.put(101, "佰");
        CHINESE_CHARS.put(102, "拾");
        CHINESE_CHARS.put(103, "");
    }

    private static String[] preProcess(String moneyString) throws InvalidInputException, OutOfRangeException {
        moneyString = moneyString.replace(" ", "").trim();
        moneyString = moneyString.replaceFirst("^0*", "");
        Pattern pattern = Pattern.compile("([0-9]+([.][0-9]*)?|[.][0-9]+)");
        Matcher matcher = pattern.matcher(moneyString);
        if (!matcher.matches()) {
            throw new InvalidInputException();
        }
        String[] moneyStringParts = new BigDecimal(moneyString).setScale(3, RoundingMode.HALF_UP).toString().split("[.]");
        if (moneyStringParts[0].length() > 12) {
            throw new OutOfRangeException();
        }
        return moneyStringParts;
    }

    private static String parseDecimal(String decimalPart) {
        char[] decimalPartCharArray = decimalPart.toCharArray();
        int decimalPartLength = decimalPartCharArray.length;
        StringBuilder decimalPartStringBuilder = new StringBuilder(1 << 3);
        for (int i = 0; i < decimalPartLength; ++i) {
            if (decimalPartCharArray[i] != 48) {
                decimalPartStringBuilder.append(CHINESE_CHARS.get(decimalPartCharArray[i] - 48)).append(CHINESE_CHARS.get(-i - 10));
            }
        }
        return decimalPartStringBuilder.toString();
    }

    private static String parseIntegerGroup(char[] subGroup) {
        StringBuilder stringBuilder = new StringBuilder(1 << 3);
        int i = 0, j = 3;
        while (i < 4 && subGroup[i] == 48) {
            ++i;
        }
        while (j >= i && subGroup[j] == 48) {
            --j;
        }
        for (int k = i; k < j + 1; ++k) {
            if (subGroup[k] != 48) {
                stringBuilder.append(CHINESE_CHARS.get(subGroup[k] - 48)).append(CHINESE_CHARS.get(100 + k));
            } else if (k == 0 || subGroup[k - 1] != 48) {
                stringBuilder.append(CHINESE_CHARS.get(0));
            }
        }
        return stringBuilder.toString();
    }

    private static String parseInteger(String integerPart) {
        char[][] integerParts = new char[3][4];
        int integerPartLength = integerPart.length();
        for (int i = 0; i < 12; ++i) {
            if (i < integerPartLength) {
                integerParts[2 - i / 4][3 - i % 4] = integerPart.charAt(integerPartLength - i - 1);
            } else {
                integerParts[2 - i / 4][3 - i % 4] = 48;
            }
        }
        StringBuilder integerPartStringBuilder = new StringBuilder(1 << 4);
        switch (2 - (integerPartLength - 1) / 4) {
            case 2 -> {
                String subgroupString2 = parseIntegerGroup(integerParts[2]);
                if (subgroupString2.isEmpty()) {
                    integerPartStringBuilder.append(CHINESE_CHARS.get(0));
                }
                integerPartStringBuilder.append(subgroupString2);
            }
            case 1 -> {
                String subgroupString1 = parseIntegerGroup(integerParts[1]);
                String subgroupString2 = parseIntegerGroup(integerParts[2]);
                integerPartStringBuilder.append(subgroupString1).append(WAN);
                if (!subgroupString2.isEmpty()) {
                    if (integerParts[2][0] == 48) {
                        integerPartStringBuilder.append(CHINESE_CHARS.get(0));
                    }
                    integerPartStringBuilder.append(subgroupString2);
                }
            }
            case 0 -> {
                String subgroupString0 = parseIntegerGroup(integerParts[0]);
                String subgroupString1 = parseIntegerGroup(integerParts[1]);
                String subgroupString2 = parseIntegerGroup(integerParts[2]);
                integerPartStringBuilder.append(subgroupString0).append(YI);
                if (!subgroupString1.isEmpty()) {
                    if (integerParts[1][0] == 48) {
                        integerPartStringBuilder.append(CHINESE_CHARS.get(0)).append(subgroupString1).append(WAN);
                    } else {
                        integerPartStringBuilder.append(subgroupString1).append(WAN);
                    }
                }
                if (!subgroupString2.isEmpty()) {
                    if (integerParts[2][0] == 48) {
                        integerPartStringBuilder.append(CHINESE_CHARS.get(0));
                    }
                    integerPartStringBuilder.append(subgroupString2);
                }
            }
        }
        return integerPartStringBuilder.append(YUAN).toString();
    }

    static String readAloud(String moneyString) throws InvalidInputException, OutOfRangeException {
        String[] moneyStrings = preProcess(moneyString);
        if (!Objects.equals(moneyStrings[0], "0") && !Objects.equals(moneyStrings[1], "000")) {
            return parseInteger(moneyStrings[0]) + parseDecimal(moneyStrings[1]);
        } else if (!Objects.equals(moneyStrings[0], "0")) {
            return parseInteger(moneyStrings[0]) + ZHENG;
        } else if (!Objects.equals(moneyStrings[1], "000")) {
            return parseDecimal(moneyStrings[1]);
        } else {
            return "零元整";
        }
    }
}

public class Main extends JFrame {
    private final JTextField input = new JTextField();
    private final JButton show = new JButton("转换金额");
    private final JButton copy = new JButton("复制结果");
    private final JLabel hint = new JLabel("请输入要转换的金额，最大不超过一千亿元：");
    private final JTextArea output = new JTextArea();
    private final JTextArea about = new JTextArea();
    private final Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();

    Main() {
        init();
        this.setTitle("输出大写金额");
        this.setSize(400, 300);
        this.setResizable(false);
        this.setVisible(true);
        this.setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    void init() {
        this.setLayout(null);
        this.add(input);
        this.add(show);
        this.add(copy);
        this.add(hint);
        this.add(output);
        this.add(about);
        hint.setBounds(55, 15, 290, 30);
        input.setBounds(50, 55, 300, 30);
        show.setBounds(80, 100, 100, 30);
        copy.setBounds(220, 100, 100, 30);
        output.setBounds(50, 145, 300, 45);
        output.setEditable(false);
        output.setLineWrap(true);
        about.setBounds(50, 195, 300, 55);
        about.setText("【说明】Java 第6章综合练习题\n【作者】陈   玄（中央财经大学）\n【版本】2.0 (35F66) + FlatLaf-2.6");
        about.setEditable(false);
        show.addActionListener(actionEvent -> {
            try {
                String result = Core.readAloud(input.getText());
                JOptionPane.showMessageDialog(this, "转换成功", "提示", JOptionPane.INFORMATION_MESSAGE);
                output.setText("【结果】人民币" + result);
            } catch (Exception exception) {
                String errorMessage = exception.getMessage();
                output.setText("");
                JOptionPane.showMessageDialog(this, errorMessage, "错误", JOptionPane.ERROR_MESSAGE);
            }
        });
        copy.addActionListener(actionEvent -> {
            StringSelection stringSelection = new StringSelection(output.getText());
            clipboard.setContents(stringSelection, null);
            JOptionPane.showMessageDialog(this, "已复制！", "提示", JOptionPane.INFORMATION_MESSAGE);
        });
    }

    public static void main(String[] args) {
        FlatIntelliJLaf.setup();
        new Main();
    }
}
