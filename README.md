# QT教育管理系统 - 初音 MMD 模型
[项目介绍视频](https://github.com/null-goudan/like_qq_login)

[部分功能讲解部分](https://tieba.baidu.com/p/9082934660?pid=150557747552&cid=0#150557747552)

教育管理界面是借用的github另一个老哥的（实在找不到网址了非常抱歉就没贴，别的都是自己写的了）
这是一个包含初音 MMD 模型的教育管理系统，项目包括智能聊天机器人、桌面宠物、小说阅读器以及爬虫页面等多个功能模块。
初音MMD模型具有9个动作
![屏幕截图 2024-07-15 091600](https://github.com/user-attachments/assets/fc18c3f8-b4b6-4b82-90dc-e7ed79a94495)
小说阅读器
![屏幕截图 2024-07-15 091934](https://github.com/user-attachments/assets/18380535-cb78-4733-9b77-87b9b9862cc2)
聊天机器人
![屏幕截图 2024-07-15 091858](https://github.com/user-attachments/assets/a0bd126a-e188-417a-8c06-92213656499e)

## 项目日志

### 2024.7.4
- 实习第一天实现了后端的接入。

### 2024.7.5
- 基本完成了页面的布置，包括标题文本框，并添加了音乐播放功能，放大貔貅。

### 2024.7.6
- 项目重构，从一个页面变成了多页面项目（包括登录页面、bot页面、爬虫页面）。
- bot 页面添加了头像和表情，登录界面上传的图像和名字可以同步到爬虫界面。

### 2024.7.7
- 得知要做教务管理系统，放弃当前项目。

### 2024.7.9
- 消沉了两天，准备不管老师重启项目。

### 2024.7.10
- 实现了桌面宠物的接入，UI 重建。

### 2024.7.11
- 完善了桌面宠物的动作，计划给桌面宠物添加定时任务和快捷打开应用的能力。

### 2024.7.12
- 桌面宠物添加快捷打开 ChatGPT 的能力，定时任务取消实现。
- 智能聊天机器人添加了识别关键词功能，能够根据关键词发出映射图片。

### 2024.7.13
- 完善了爬虫页面，三个页面基本完成，聊天机器人页面较为完善，继续优化聊天机器人页面。

### 2024.7.14
- 添加了文本阅读器页面，可以更好地阅读 `.txt` 文件和实现 `.md` 文件的 Markdown 渲染。

### 2024.7.15
- 给智愈机器人添加了主动识别用户特殊问题的能力。

## 使用方法（智能聊天机器人）

1. 使用 Qt Creator 打开 `build` 文件夹中的 `.pro` 文件。
2. 修改 main.cpp 中的目录中自己的路径
3. 注意使用的版本为qt6.6.3,5几的版本需要自己更改相应内容
4. 请将音量调低，因为有较大音量的大貔貅（已经关闭）。

## 后续机器人聊天页面更新目标

1. ~~聊天时显示图片而非文字或表情，7.5号已完成。~~
2. 随机在聊天文字后插入颜文字，尚未实现。
3. 给音乐播放器添加进度条和歌词（需要更新 Qt 版本）。 
4. 页面布局调整。
5. 添加关键词识别和图片回复功能。

## 后续功能计划

### 1. 登录页面
- 实现一个简单的登录页面，支持修改密码和账号（但验证码功能暂不实现）。
- ~~实现 QQ 头像更换，并同步显示到聊天页面。~~
- 支持用户头像和名称的同步显示。

### 2. 聊天页面
- 实现类似 QQ 的聊天页面，将智愈机器人嵌入其中（目前已废弃）。
- 在我下一个项目java网站里面已经实现了，借用了potize老哥写的网站的思路
- 可以支持语音聊天和语音回复（不过在我发的压缩包里面这些功能被注释了需要得自己下载gpt_sovits和Porcupine）


### 3. 爬虫页面
- 用于爬取美女图片，供聊天机器人使用（。。。半年时间我的爬虫能力也从零变成能稍有作为了）。

### 4. 小说阅读页面
- 主要用于看历史聊天记录
- 支持MarkDown格式

### 5. 桌面宠物
- 实现多个动作、快捷打开应用（例如 QQ 和 ChatGPT）。

### 6. 教务管理系统
- 计划中，参考现有项目进行开发。

## 参考资料

- [QQ的登录页面](https://github.com/null-goudan/like_qq_login)
- [YYBple式初音ミク: ボトル衣装](https://bowlroll.net/file/320643)
- [用QT实现一个简单的桌面宠物](https://juejin.cn/post/7167341326949138446?searchId=20230906203601C7668722802978AC5929)
- [MMD简易入门教程](https://www.bilibili.com/read/cv29470907/?jump_opus=1)

## 项目结束

2024年7月17日，项目结束，最终形成了一个教育管理系统，并且包含了初音的 MMD 模型。

**注意：** 项目中的路径设置可能需要手动调整，特别是在 `main.app` 和数据库连接的地方，请根据自己的文件路径进行修改。

