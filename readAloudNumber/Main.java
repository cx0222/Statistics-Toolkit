package com.nova.cx0222.homework;

// 陈  玄 2020311462

import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.HashMap;
import java.util.Objects;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

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

public class Main {
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

    public static String[] preProcess(String moneyString) throws InvalidInputException, OutOfRangeException {
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

    public static String parseDecimal(String decimalPart) {
        System.out.println("小数部分：" + decimalPart);
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

    public static String parseIntegerGroup(char[] subGroup) {
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

    public static String parseInteger(String integerPart) {
        System.out.println("整数部分：" + integerPart);
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

    public static String readAloud(String moneyString) throws InvalidInputException, OutOfRangeException {
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

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String moneyString = scanner.next();
        try {
            String result = readAloud(moneyString);
            System.out.println("人民币" + result);
        } catch (Exception exception) {
            System.err.println(exception.getMessage());
            exception.getStackTrace();
        }
    }
}
