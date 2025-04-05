

# Namespace qqmusic



[**Namespace List**](namespaces.md) **>** [**qqmusic**](namespaceqqmusic.md)


















## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**crypto**](namespaceqqmusic_1_1crypto.md) <br> |
| namespace | [**details**](namespaceqqmusic_1_1details.md) <br> |
| namespace | [**utils**](namespaceqqmusic_1_1utils.md) <br>_简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖_ `nlohmann::json` _实现 目前不支持路径嵌套和_`Expires` _和_`HttpOnly` _这样的特性_ |


## Classes

| Type | Name |
| ---: | :--- |
| class | [**BaseMediaFileType**](classqqmusic_1_1BaseMediaFileType.md) <br>_媒体文件类型基类_  |
| class | [**EncryptedSongFileType**](classqqmusic_1_1EncryptedSongFileType.md) <br>_加密歌曲文件类型_  |
| struct | [**Lyric**](structqqmusic_1_1Lyric.md) <br>_歌词_  |
| struct | [**MvUrl**](structqqmusic_1_1MvUrl.md) <br>_Mv url封裝_  |
| struct | [**PhoneLoginResult**](structqqmusic_1_1PhoneLoginResult.md) <br> |
| struct | [**QRCode**](structqqmusic_1_1QRCode.md) <br>_二维码信息_  |
| class | [**QRCodeLoginEvent**](classqqmusic_1_1QRCodeLoginEvent.md) <br>_二维码登陆事件_  |
| struct | [**QRCodeLoginResult**](structqqmusic_1_1QRCodeLoginResult.md) <br>_二维码登陆结果封装_  |
| class | [**SongFileType**](classqqmusic_1_1SongFileType.md) <br>_未加密的歌曲文件类枚举_  |
| struct | [**SongInfo**](structqqmusic_1_1SongInfo.md) <br> |
| class | [**TabType**](classqqmusic_1_1TabType.md) <br>_标签类_  |


## Public Types

| Type | Name |
| ---: | :--- |
| enum int | [**AreaType**](#enum-areatype)  <br>_区域类型枚举_  |
| enum int | [**CoverSize**](#enum-coversize)  <br>_封面类型枚举_  |
| enum int | [**GenderType**](#enum-gendertype)  <br>_性别类型枚举_  |
| enum int | [**GenreType**](#enum-genretype)  <br>_风格类型枚举_  |
| enum int | [**IndexType**](#enum-indextype)  <br>_索引类型枚举, 按首字母筛选时需要_  |
| enum int | [**PhoneLoginEvent**](#enum-phoneloginevent)  <br>_手机号登陆状态枚举_  |
| enum  | [**QRLoginType**](#enum-qrlogintype)  <br>_登陆类型枚举_  |
| typedef [**Result**](structResult.md)&lt; T, [**qqmusic::utils::Exception**](classqqmusic_1_1utils_1_1Exception.md) &gt; | [**Result**](#typedef-result)  <br>_Rust风格的返回结果封装, 正确结果和异常共用同一个渠道返回_  |
| enum unsigned | [**SearchType**](#enum-searchtype)  <br>_搜索类型枚举_  |
| typedef boost::asio::awaitable&lt; T &gt; | [**Task**](#typedef-task)  <br>_通用异步任务_  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|  qqmusic::Task&lt; qqmusic::Result&lt; [**QRCodeLoginResult**](structqqmusic_1_1QRCodeLoginResult.md) &gt; &gt; | [**check\_qq\_qr**](#function-check_qq_qr) ([**QRCode**](structqqmusic_1_1QRCode.md) & qrc) <br>_检查QQ二维码状态_  |
|  qqmusic::Task&lt; qqmusic::Result&lt; [**QRCodeLoginResult**](structqqmusic_1_1QRCodeLoginResult.md) &gt; &gt; | [**check\_wx\_qr**](#function-check_wx_qr) ([**QRCode**](structqqmusic_1_1QRCode.md) & qrc) <br>_检查微信二维码状态_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**general\_search**](#function-general_search) (std::string\_view keyword, unsigned num=15, unsigned page=1, bool highlight=true) <br>_综合搜索_  |
|  std::string | [**get\_album\_cover\_url**](#function-get_album_cover_url) (std::string\_view mid, CoverSize size) <br>_获取专辑封面url_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_album\_detail**](#function-get_album_detail) (std::string\_view album\_mid) <br>_从mid获取专辑信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_album\_detail**](#function-get_album_detail) (uint64\_t album\_id) <br>_从id获取专辑信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_album\_list**](#function-get_album_list) (std::string\_view mid, unsigned number=10, unsigned begin=0) <br>_获取歌手专辑列表_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_album\_list\_all**](#function-get_album_list_all) (std::string\_view mid) <br>_获取歌手所有专辑_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_album\_songs**](#function-get_album_songs) (std::string\_view album\_mid, unsigned int num=10, unsigned int page=1) <br>_从mid获取专辑歌曲列表_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_album\_songs**](#function-get_album_songs) (uint64\_t album\_id, unsigned int num=10, unsigned int page=1) <br>_从id获取专辑歌曲列表_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_created\_songlist**](#function-get_created_songlist) (uint64\_t musicid, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_获取创建的歌单列表_  |
|  Task&lt; [**Result**](structResult.md)&lt; std::string &gt; &gt; | [**get\_euin**](#function-get_euin) (uint64\_t musicid) <br>_由musicid获取euin_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_fans**](#function-get_fans) (std::string\_view euin, unsigned page=1, unsigned num=10, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_粉丝数量_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_fav\_album**](#function-get_fav_album) (std::string\_view euin, unsigned page=1, unsigned num=10, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_获取收藏专辑_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_fav\_mv**](#function-get_fav_mv) (std::string\_view euin, unsigned page=1, unsigned num=10, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_获取收藏MV_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_fav\_num**](#function-get_fav_num) (std::span&lt; uint64\_t &gt; id\_list, bool readable=true) <br>_获取歌曲的收藏量_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_fav\_song**](#function-get_fav_song) (std::string\_view euin, unsigned page=1, unsigned num=10, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_获取收藏歌曲_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_fav\_songlist**](#function-get_fav_songlist) (std::string\_view euin, unsigned page=1, unsigned num=10, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_获取收藏歌单_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_follow\_singers**](#function-get_follow_singers) (std::string\_view euin, unsigned page=1, unsigned num=10, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_获取关注的歌手列表_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_follow\_usr**](#function-get_follow_usr) (std::string\_view euin, unsigned page=1, unsigned num=10, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_获取关注用户列表_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_friend\_list**](#function-get_friend_list) (unsigned page=1, unsigned num=10, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_获取好友列表_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_homepage**](#function-get_homepage) (std::string\_view euin, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_获取用户主页信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_labels**](#function-get_labels) (uint64\_t songid) <br>_获取歌曲标签信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; [**Lyric**](structqqmusic_1_1Lyric.md) &gt; &gt; | [**get\_lyric**](#function-get_lyric) (std::string\_view mid, bool qrc=false, bool trans=false, bool roma=false) <br>_获取歌曲的歌词_  |
|  Task&lt; [**Result**](structResult.md)&lt; [**Lyric**](structqqmusic_1_1Lyric.md) &gt; &gt; | [**get\_lyric**](#function-get_lyric) (uint64\_t id, bool qrc=false, bool trans=false, bool roma=false) <br>_获取歌曲的歌词_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_music\_gene**](#function-get_music_gene) (std::string\_view euin, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_获取音乐基因_  |
|  Task&lt; [**Result**](structResult.md)&lt; uint64\_t &gt; &gt; | [**get\_musicid**](#function-get_musicid) (std::string\_view euin) <br>_由euin获取musicid_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_mv\_detail**](#function-get_mv_detail) (std::span&lt; std::string &gt; vids) <br>_获取一组MV的信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_mv\_list**](#function-get_mv_list) (std::string\_view mid, unsigned number=10, unsigned begin=0) <br>_获取歌手MV列表_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_mv\_list\_all**](#function-get_mv_list_all) (std::string\_view mid) <br>_获取歌手所有MV_  |
|  Task&lt; [**Result**](structResult.md)&lt; std::vector&lt; [**MvUrl**](structqqmusic_1_1MvUrl.md) &gt; &gt; &gt; | [**get\_mv\_urls**](#function-get_mv_urls) (std::span&lt; std::string &gt; vids) <br>_获取mv url_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_other\_version**](#function-get_other_version) (std::string\_view mid) <br>_获取同名歌曲的其他版本_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_other\_version**](#function-get_other_version) (uint64\_t id) <br>_获取同名歌曲的其他版本_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_producer\_info**](#function-get_producer_info) (std::string\_view mid) <br>_获取歌曲制作者信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_producer\_info**](#function-get_producer_info) (uint64\_t id) <br>_获取歌曲制作者信息_  |
|  qqmusic::Task&lt; qqmusic::Result&lt; [**QRCode**](structqqmusic_1_1QRCode.md) &gt; &gt; | [**get\_qrcode**](#function-get_qrcode) (QRLoginType login\_type) <br>_获取登陆二维码_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_related\_mv**](#function-get_related_mv) (uint64\_t songid, std::optional&lt; std::string &gt; last\_mvid=std::nullopt) <br>_获取相关MV_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_related\_songlist**](#function-get_related_songlist) (uint64\_t songid) <br>_获取相关歌单列表_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_sheet**](#function-get_sheet) (std::string\_view mid) <br>_获取曲谱信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_similar\_singers**](#function-get_similar_singers) (std::string\_view mid, unsigned number=10) <br>_获取相似歌手信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_similar\_songs**](#function-get_similar_songs) (uint64\_t songid) <br>_获取相似歌曲列表_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_singer\_desc**](#function-get_singer_desc) (std::span&lt; std::string &gt; mids, unsigned number=10) <br>_获取多个歌手的简介_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_singer\_info**](#function-get_singer_info) (std::string\_view mid) <br>_获取歌手基本信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_singer\_list**](#function-get_singer_list) (AreaType area=AreaType::ALL, GenderType gender=GenderType::ALL, GenreType genre=GenreType::ALL) <br>_获取符合筛选条件的歌手信息表单_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_singer\_list\_index**](#function-get_singer_list_index) (AreaType area=AreaType::ALL, GenderType gender=GenderType::ALL, GenreType genre=GenreType::ALL, IndexType index=IndexType::ALL, unsigned sin=0, unsigned cur\_page=1) <br>_筛选获得歌曲歌单原始数据_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_singer\_list\_index\_all**](#function-get_singer_list_index_all) (AreaType area=AreaType::ALL, GenderType gender=GenderType::ALL, GenreType genre=GenreType::ALL, IndexType index=IndexType::ALL) <br>_获取所有符合条件的歌单原始数据_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_song\_detail**](#function-get_song_detail) (std::string\_view mid) <br>_获取歌曲详细信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_song\_detail**](#function-get_song_detail) (uint64\_t id) <br>_获取歌曲详细信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; std::vector&lt; [**SongInfo**](structqqmusic_1_1SongInfo.md) &gt; &gt; &gt; | [**get\_song\_urls**](#function-get_song_urls) (std::span&lt; std::string &gt; mids, std::unique\_ptr&lt; [**BaseMediaFileType**](classqqmusic_1_1BaseMediaFileType.md) &gt; file\_type=std::make\_unique&lt; [**SongFileType**](classqqmusic_1_1SongFileType.md) &gt;(), std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_批量获取歌曲播放链接_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_songlist**](#function-get_songlist) (uint64\_t dissid, unsigned dirid=0) <br>_获取歌单列表_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_songlist\_detail**](#function-get_songlist_detail) (uint64\_t dissid, unsigned dirid=0, unsigned num=10, unsigned page=1, bool only\_song=false, bool tag=true, bool user\_info=true) <br>_获取歌单信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_songs\_list**](#function-get_songs_list) (std::string\_view mid, unsigned num=10, unsigned begin=0) <br>_获取歌手歌曲原始数据_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_songs\_list\_all**](#function-get_songs_list_all) (std::string\_view mid) <br>_获取歌手的所有歌曲_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_tab\_detail**](#function-get_tab_detail) (std::string\_view mid, [**TabType**](classqqmusic_1_1TabType.md) type, unsigned page=1, unsigned num=10) <br>_返回标签详细信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_toplist\_category**](#function-get_toplist_category) () <br>_获取所有排行榜_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_toplist\_item\_detail**](#function-get_toplist_item_detail) (unsigned top\_id, unsigned num=10, unsigned page=1, bool tag=true) <br>_获取排行榜条目信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; std::string &gt; &gt; | [**get\_try\_url**](#function-get_try_url) (std::string\_view mid, std::string\_view vs) <br>_获取试听链接_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**get\_vip\_info**](#function-get_vip_info) (std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_获取当前账号vip信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**hotkey**](#function-hotkey) () <br>_获取搜索热词_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**keyword\_complete**](#function-keyword_complete) (std::string\_view keyword) <br>_搜索词联想补全_  |
|  qqmusic::Task&lt; qqmusic::Result&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; &gt; | [**phone\_authorize**](#function-phone_authorize) (std::string\_view phone, std::string\_view auth\_code, std::string\_view country\_code="86") <br>_手机号认证_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**query\_song**](#function-query_song) (std::span&lt; std::string &gt; mids) <br>_批量查询歌曲信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**query\_song**](#function-query_song) (std::span&lt; uint64\_t &gt; ids) <br>_批量查询歌曲信息_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**quick\_search**](#function-quick_search) (std::string\_view keyword) <br>_快速搜索_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**search\_by\_type**](#function-search_by_type) (std::string\_view keyword, SearchType type=SearchType::SONG, unsigned num\_per\_page=10, unsigned page=1, bool highlight=true) <br>_按类型搜索_  |
|  qqmusic::Task&lt; qqmusic::Result&lt; [**PhoneLoginResult**](structqqmusic_1_1PhoneLoginResult.md) &gt; &gt; | [**send\_authcode**](#function-send_authcode) (std::string\_view phone, std::string\_view country\_code="86") <br>_向指定手机号发送验证码_  |
|  Task&lt; [**Result**](structResult.md)&lt; bool &gt; &gt; | [**songlist\_add\_songs**](#function-songlist_add_songs) (uint64\_t dirid, std::span&lt; uint64\_t &gt; song\_ids, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_向歌单内添加歌曲_  |
|  Task&lt; [**Result**](structResult.md)&lt; nlohmann::json &gt; &gt; | [**songlist\_create**](#function-songlist_create) (std::string\_view dirname, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_创建歌单_  |
|  Task&lt; [**Result**](structResult.md)&lt; bool &gt; &gt; | [**songlist\_delete**](#function-songlist_delete) (uint64\_t dirid, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_删除歌单_  |
|  Task&lt; [**Result**](structResult.md)&lt; bool &gt; &gt; | [**songlist\_delete\_songs**](#function-songlist_delete_songs) (uint64\_t dirid, std::span&lt; uint64\_t &gt; song\_ids, std::optional&lt; [**utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) &gt; credential=std::nullopt) <br>_删除歌单内指定歌曲_  |




























## Public Types Documentation




### enum AreaType 

_区域类型枚举_ 
```C++
enum qqmusic::AreaType {
    ALL = -100,
    CHINA = 200,
    TAIWAN = 2,
    AMERICA = 5,
    JAPAN = 4,
    KOREA = 3
};
```




<hr>



### enum CoverSize 

_封面类型枚举_ 
```C++
enum qqmusic::CoverSize {
    SMALL = 150,
    DEFAULT = 300,
    MIDIUM = 500,
    LARGE = 800
};
```




<hr>



### enum GenderType 

_性别类型枚举_ 
```C++
enum qqmusic::GenderType {
    ALL = -100,
    MALE = 0,
    FEMALE = 1,
    GROUP = 2
};
```




<hr>



### enum GenreType 

_风格类型枚举_ 
```C++
enum qqmusic::GenreType {
    ALL = -100,
    POP = 7,
    RAP = 3,
    CHINESE_STYLE = 19,
    ROCK = 4,
    ELECTRONIC = 2,
    FOLK = 8,
    R_AND_B = 11,
    ETHNIC = 37,
    LIGHT_MUSIC = 93,
    JAZZ = 14,
    CLASSICAL = 33,
    COUNTRY = 13,
    BLUES = 10
};
```




<hr>



### enum IndexType 

_索引类型枚举, 按首字母筛选时需要_ 
```C++
enum qqmusic::IndexType {
    A = 1,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    HASH,
    ALL = -100
};
```




<hr>



### enum PhoneLoginEvent 

_手机号登陆状态枚举_ 
```C++
enum qqmusic::PhoneLoginEvent {
    SEND = 0,
    CAPTCHA = 20276,
    FREQUENCY = 100001,
    OTHER = -1
};
```




<hr>



### enum QRLoginType 

_登陆类型枚举_ 
```C++
enum qqmusic::QRLoginType {
    QQ,
    WX
};
```




<hr>



### typedef Result 

_Rust风格的返回结果封装, 正确结果和异常共用同一个渠道返回_ 
```C++
using qqmusic::Result = typedef Result<T, qqmusic::utils::Exception>;
```



使用方法: 
```C++
Result<int> func(int a);

int main() {
    auto res = func(114514);
    if (res.isErr()) {
        // error occurred in func, unwrap the error
        std::cout << res.unwrapErr().what() << std::endl;
    }
    // func returned normally, unwrap the result
    int x = res.unwrap();
    return 0;
}
```
 


        

<hr>



### enum SearchType 

_搜索类型枚举_ 
```C++
enum qqmusic::SearchType {
    SONG = 0,
    SINGER = 1,
    ALBUM = 2,
    SONG_LIST = 3,
    MV = 4,
    LYRIC = 7,
    USER = 8,
    AUDIO_ALBUM = 15,
    AUDIO = 18
};
```




<hr>



### typedef Task 

_通用异步任务_ 
```C++
using qqmusic::Task = typedef boost::asio::awaitable<T>;
```




<hr>
## Public Functions Documentation




### function check\_qq\_qr 

_检查QQ二维码状态_ 
```C++
qqmusic::Task< qqmusic::Result< QRCodeLoginResult > > qqmusic::check_qq_qr (
    QRCode & qrc
) 
```





**Parameters:**


* `qrc` QQ二维码



**Returns:**

包含正确结果的`Task< Result < QRCodeLoginResult >>`. 如果登陆成功会包含Credential




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function check\_wx\_qr 

_检查微信二维码状态_ 
```C++
qqmusic::Task< qqmusic::Result< QRCodeLoginResult > > qqmusic::check_wx_qr (
    QRCode & qrc
) 
```





**Parameters:**


* `qrc` 微信二维码



**Returns:**

包含正确结果的`Task< Result < QRCodeLoginResult >>`. 如果登陆成功会包含Credential




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function general\_search 

_综合搜索_ 
```C++
Task< Result < nlohmann::json > > qqmusic::general_search (
    std::string_view keyword,
    unsigned num=15,
    unsigned page=1,
    bool highlight=true
) 
```





**Parameters:**


* `keyword` 搜索词 
* `num` 每页结果数, 默认`15` 
* `page` 页数, 默认`1` 
* `highlight` 是否高亮, 默认`true`



**Returns:**

Json格式搜索结果




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_album\_cover\_url 

_获取专辑封面url_ 
```C++
std::string qqmusic::get_album_cover_url (
    std::string_view mid,
    CoverSize size
) 
```





**Parameters:**


* `mid` 歌曲mid 
* `size` 歌曲封面尺寸



**Returns:**

std::string格式歌曲封面的url 





        

<hr>



### function get\_album\_detail 

_从mid获取专辑信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_album_detail (
    std::string_view album_mid
) 
```





**Parameters:**


* `album_mid` 专辑mid



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_album\_detail 

_从id获取专辑信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_album_detail (
    uint64_t album_id
) 
```





**Parameters:**


* `album_id` 专辑id



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_album\_list 

_获取歌手专辑列表_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_album_list (
    std::string_view mid,
    unsigned number=10,
    unsigned begin=0
) 
```





**Parameters:**


* `mid` 歌手mid 
* `number` 单次获取数据量, 默认`10` 
* `begin` 从第几个开始



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_album\_list\_all 

_获取歌手所有专辑_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_album_list_all (
    std::string_view mid
) 
```





**Parameters:**


* `mid` 歌手mid



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_album\_songs 

_从mid获取专辑歌曲列表_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_album_songs (
    std::string_view album_mid,
    unsigned int num=10,
    unsigned int page=1
) 
```





**Parameters:**


* `album_mid` 专辑mid



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_album\_songs 

_从id获取专辑歌曲列表_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_album_songs (
    uint64_t album_id,
    unsigned int num=10,
    unsigned int page=1
) 
```





**Parameters:**


* `album_id` 专辑id



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_created\_songlist 

_获取创建的歌单列表_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_created_songlist (
    uint64_t musicid,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `musicid` 
* `credential` 凭据信息 




        

<hr>



### function get\_euin 

_由musicid获取euin_ 
```C++
Task< Result < std::string > > qqmusic::get_euin (
    uint64_t musicid
) 
```





**Parameters:**


* `musicid` MusicID, 通常是QQ号



**Returns:**

Euin 





        

<hr>



### function get\_fans 

_粉丝数量_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_fans (
    std::string_view euin,
    unsigned page=1,
    unsigned num=10,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `encrypt_uin` 
* `page` 页码, 默认`1` 
* `num` 返回数量, 默认`10` 
* `credential` 凭据信息 




        

<hr>



### function get\_fav\_album 

_获取收藏专辑_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_fav_album (
    std::string_view euin,
    unsigned page=1,
    unsigned num=10,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `encrypt_uin` 
* `page` 页码, 默认`1` 
* `num` 返回数量, 默认`10` 
* `credential` 凭据信息 




        

<hr>



### function get\_fav\_mv 

_获取收藏MV_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_fav_mv (
    std::string_view euin,
    unsigned page=1,
    unsigned num=10,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `encrypt_uin` 
* `page` 页码, 默认`1` 
* `num` 返回数量, 默认`10` 
* `credential` 凭据信息 




        

<hr>



### function get\_fav\_num 

_获取歌曲的收藏量_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_fav_num (
    std::span< uint64_t > id_list,
    bool readable=true
) 
```





**Parameters:**


* `id_list` 歌曲id列表 
* `readable` 是否人类可读

示例:



```C++
readable == false 返回: {'438910555': 1000001}
readable == true 返回:  {'438910555': '550w+'}
```





**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_fav\_song 

_获取收藏歌曲_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_fav_song (
    std::string_view euin,
    unsigned page=1,
    unsigned num=10,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `encrypt_uin` 
* `page` 页码, 默认`1` 
* `num` 返回数量, 默认`10` 
* `credential` 凭据信息 




        

<hr>



### function get\_fav\_songlist 

_获取收藏歌单_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_fav_songlist (
    std::string_view euin,
    unsigned page=1,
    unsigned num=10,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `encrypt_uin` 
* `page` 页码, 默认`1` 
* `num` 返回数量, 默认`10` 
* `credential` 凭据信息 




        

<hr>



### function get\_follow\_singers 

_获取关注的歌手列表_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_follow_singers (
    std::string_view euin,
    unsigned page=1,
    unsigned num=10,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `encrypt_uin` 
* `page` 页码, 默认`1` 
* `num` 返回数量, 默认`10` 
* `credential` 凭据信息 




        

<hr>



### function get\_follow\_usr 

_获取关注用户列表_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_follow_usr (
    std::string_view euin,
    unsigned page=1,
    unsigned num=10,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `encrypt_uin` 
* `page` 页码, 默认`1` 
* `num` 返回数量, 默认`10` 
* `credential` 凭据信息 




        

<hr>



### function get\_friend\_list 

_获取好友列表_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_friend_list (
    unsigned page=1,
    unsigned num=10,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `page` 页码, 默认`1` 
* `num` 返回数量, 默认`10` 
* `credential` 凭据信息 




        

<hr>



### function get\_homepage 

_获取用户主页信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_homepage (
    std::string_view euin,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `euin` encrypt\_uin 
* `credential` 凭据信息



**Returns:**

用户主页信息, 包含音乐基因, 歌单等 





        

<hr>



### function get\_labels 

_获取歌曲标签信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_labels (
    uint64_t songid
) 
```





**Parameters:**


* `id` 歌曲id



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_lyric 

_获取歌曲的歌词_ 
```C++
Task< Result < Lyric > > qqmusic::get_lyric (
    std::string_view mid,
    bool qrc=false,
    bool trans=false,
    bool roma=false
) 
```





**Parameters:**


* `mid` 歌曲mid 
* `qrc` lyric是否是xml格式, 默认false 
* `trans` 是否附带翻译 
* `roma` 是否附带罗马音



**Returns:**

原始的歌词字符串, qrc歌词仅经过解码, 还是原始的xml格式




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_lyric 

_获取歌曲的歌词_ 
```C++
Task< Result < Lyric > > qqmusic::get_lyric (
    uint64_t id,
    bool qrc=false,
    bool trans=false,
    bool roma=false
) 
```





**Parameters:**


* `id` 歌曲id 
* `qrc` lyric是否是xml格式, 默认false 
* `trans` 是否附带翻译 
* `roma` 是否附带罗马音



**Returns:**

原始的歌词字符串, qrc歌词仅经过解码, 还是原始的xml格式




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_music\_gene 

_获取音乐基因_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_music_gene (
    std::string_view euin,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `euin` encrypt\_uin 
* `credential` 凭据信息 




        

<hr>



### function get\_musicid 

_由euin获取musicid_ 
```C++
Task< Result < uint64_t > > qqmusic::get_musicid (
    std::string_view euin
) 
```





**Parameters:**


* `euin` encrypt\_uin



**Returns:**

musicid 





        

<hr>



### function get\_mv\_detail 

_获取一组MV的信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_mv_detail (
    std::span< std::string > vids
) 
```





**Parameters:**


* `vids` 待查询的MV vid列表



**Returns:**

MV信息json




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_mv\_list 

_获取歌手MV列表_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_mv_list (
    std::string_view mid,
    unsigned number=10,
    unsigned begin=0
) 
```





**Parameters:**


* `mid` 歌手mid 
* `number` 单次获取数据量, 默认`10` 
* `begin` 从第几个开始



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_mv\_list\_all 

_获取歌手所有MV_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_mv_list_all (
    std::string_view mid
) 
```





**Parameters:**


* `mid` 歌手mid



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_mv\_urls 

_获取mv url_ 
```C++
Task< Result < std::vector< MvUrl > > > qqmusic::get_mv_urls (
    std::span< std::string > vids
) 
```





**Parameters:**


* `vids` 待查询的MV vid列表



**Returns:**

MvUrl列表




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_other\_version 

_获取同名歌曲的其他版本_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_other_version (
    std::string_view mid
) 
```





**Parameters:**


* `mid` 歌曲mid



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_other\_version 

_获取同名歌曲的其他版本_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_other_version (
    uint64_t id
) 
```





**Parameters:**


* `id` 歌曲id



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_producer\_info 

_获取歌曲制作者信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_producer_info (
    std::string_view mid
) 
```





**Parameters:**


* `mid` 歌曲mid



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

可能返回JSON的`null`, 即没有制作者信息 




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_producer\_info 

_获取歌曲制作者信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_producer_info (
    uint64_t id
) 
```





**Parameters:**


* `id` 歌曲id



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

可能返回JSON的`null`, 即没有制作者信息 




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_qrcode 

_获取登陆二维码_ 
```C++
qqmusic::Task< qqmusic::Result< QRCode > > qqmusic::get_qrcode (
    QRLoginType login_type
) 
```





**Parameters:**


* `login_type` 登陆类型



**Returns:**

包含正确结果的`Task< Result < QRCode >>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_related\_mv 

_获取相关MV_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_related_mv (
    uint64_t songid,
    std::optional< std::string > last_mvid=std::nullopt
) 
```





**Parameters:**


* `id` 歌曲id



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_related\_songlist 

_获取相关歌单列表_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_related_songlist (
    uint64_t songid
) 
```





**Parameters:**


* `id` 歌曲id



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_sheet 

_获取曲谱信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_sheet (
    std::string_view mid
) 
```





**Parameters:**


* `mid` 歌曲mid



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_similar\_singers 

_获取相似歌手信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_similar_singers (
    std::string_view mid,
    unsigned number=10
) 
```





**Parameters:**


* `mids` 歌手mid 
* `number` 相似歌手数量, 默认为`0`



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**See also:** [**TabType**](classqqmusic_1_1TabType.md) 


**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_similar\_songs 

_获取相似歌曲列表_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_similar_songs (
    uint64_t songid
) 
```





**Parameters:**


* `id` 歌曲id



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_singer\_desc 

_获取多个歌手的简介_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_singer_desc (
    std::span< std::string > mids,
    unsigned number=10
) 
```





**Parameters:**


* `mids` 歌手mid列表 
* `number` 相似歌手数量, 默认为`0`



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**See also:** [**TabType**](classqqmusic_1_1TabType.md) 


**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_singer\_info 

_获取歌手基本信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_singer_info (
    std::string_view mid
) 
```





**Parameters:**


* `歌手mid` 



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_singer\_list 

_获取符合筛选条件的歌手信息表单_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_singer_list (
    AreaType area=AreaType::ALL,
    GenderType gender=GenderType::ALL,
    GenreType genre=GenreType::ALL
) 
```



获取JSON格式的歌手信息, 可以使用area, gender和genre筛选




**Parameters:**


* `area` 区域信息筛选, 默认`AreaType::ALL`. 
* `gender` 性别信息筛选, 默认`GenderType::ALL`. 
* `genre` 风格信息筛选, 默认`GenreType::ALL`.



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**See also:** AreaType, GenderType, GenreType 


**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_singer\_list\_index 

_筛选获得歌曲歌单原始数据_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_singer_list_index (
    AreaType area=AreaType::ALL,
    GenderType gender=GenderType::ALL,
    GenreType genre=GenreType::ALL,
    IndexType index=IndexType::ALL,
    unsigned sin=0,
    unsigned cur_page=1
) 
```





**Parameters:**


* `area` 区域信息筛选, 默认`AreaType::ALL`. 
* `gender` 性别信息筛选, 默认`GenderType::ALL`. 
* `genre` 风格信息筛选, 默认`GenreType::ALL`. 
* `sin` 跳过的条目数量, 默认`0` 
* `cur_page` 当前页面的索引, 默认`1`



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**See also:** AreaType, GenderType, GenreType, IndexType 


**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_singer\_list\_index\_all 

_获取所有符合条件的歌单原始数据_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_singer_list_index_all (
    AreaType area=AreaType::ALL,
    GenderType gender=GenderType::ALL,
    GenreType genre=GenreType::ALL,
    IndexType index=IndexType::ALL
) 
```





**Parameters:**


* `area` 区域信息筛选, 默认`AreaType::ALL`. 
* `gender` 性别信息筛选, 默认`GenderType::ALL`. 
* `genre` 风格信息筛选, 默认`GenreType::ALL`. 
* `index` 索引信息筛选, 默认`IndexType::ALL`.



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**See also:** AreaType, GenderType, GenreType, IndexType 


**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_song\_detail 

_获取歌曲详细信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_song_detail (
    std::string_view mid
) 
```





**Parameters:**


* `mid` 歌曲mid



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_song\_detail 

_获取歌曲详细信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_song_detail (
    uint64_t id
) 
```





**Parameters:**


* `id` 歌曲id



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_song\_urls 

_批量获取歌曲播放链接_ 
```C++
Task< Result < std::vector< SongInfo > > > qqmusic::get_song_urls (
    std::span< std::string > mids,
    std::unique_ptr< BaseMediaFileType > file_type=std::make_unique< SongFileType >(),
    std::optional< utils::Credential > credential=std::nullopt
) 
```



@params mids 歌曲mid列表 @params file\_type 歌曲文件类型, 默认MP3\_128类型 @params credential 凭证信息


调用示例: 
```C++
std::vector<std::string> mids{"002n5zzy3VVGau"};

// 未加密类型
auto res = co_await get_song_urls(mids,
    std::make_unique<SongFileType>(SongFileType::ACC_192));

// 加密文件类型 
auto res = co_await get_song_urls(mids,
    std::make_unique<EncryptedSongFileType>(EncryptedSongFileType::MASTER));
```





**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**See also:** [**BaseMediaFileType**](classqqmusic_1_1BaseMediaFileType.md), [**SongFileType**](classqqmusic_1_1SongFileType.md), [**EncryptedSongFileType**](classqqmusic_1_1EncryptedSongFileType.md) 


**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_songlist 

_获取歌单列表_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_songlist (
    uint64_t dissid,
    unsigned dirid=0
) 
```





**Parameters:**


* `dissid` 歌单ID 
* `dirid` 歌单dirid, 默认为0



**Returns:**

Json格式歌单列表




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_songlist\_detail 

_获取歌单信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_songlist_detail (
    uint64_t dissid,
    unsigned dirid=0,
    unsigned num=10,
    unsigned page=1,
    bool only_song=false,
    bool tag=true,
    bool user_info=true
) 
```





**Parameters:**


* `dissid` 歌单ID 
* `dirid` 歌单drid, 默认为`0` 
* `num` 返回songlist元素数量, 默认为`10` 
* `page` 第几页, 默认为`1` 
* `only_song` 是否仅返回歌曲信息, 默认`false` 
* `tag` 是否返回歌单标签信息, 默认`true` 
* `user_info` 是否返回歌单创建者的用户信息, 默认`true`



**Returns:**

Json格式的歌单信息




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_songs\_list 

_获取歌手歌曲原始数据_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_songs_list (
    std::string_view mid,
    unsigned num=10,
    unsigned begin=0
) 
```





**Parameters:**


* `mid` 歌手mid 
* `num` 每次获取的最大数量, 默认`10`, 最大`30` 
* `begin` 从第几个开始



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_songs\_list\_all 

_获取歌手的所有歌曲_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_songs_list_all (
    std::string_view mid
) 
```





**Parameters:**


* `mid` 歌手mid



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_tab\_detail 

_返回标签详细信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_tab_detail (
    std::string_view mid,
    TabType type,
    unsigned page=1,
    unsigned num=10
) 
```





**Parameters:**


* `mid` 歌手mid 
* `type` 标签信息 
* `page` 页码 
* `num` 每页返回数量



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**See also:** [**TabType**](classqqmusic_1_1TabType.md) 


**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_toplist\_category 

_获取所有排行榜_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_toplist_category () 
```




<hr>



### function get\_toplist\_item\_detail 

_获取排行榜条目信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_toplist_item_detail (
    unsigned top_id,
    unsigned num=10,
    unsigned page=1,
    bool tag=true
) 
```





**Parameters:**


* `top_id` 排行榜ID 
* `num` 返回数量, 默认为`10` 
* `page` 页码, 默认为`0` 
* `tag` 是否返回歌曲标签, 默认为`true`



**Returns:**







        

<hr>



### function get\_try\_url 

_获取试听链接_ 
```C++
Task< Result < std::string > > qqmusic::get_try_url (
    std::string_view mid,
    std::string_view vs
) 
```





**Parameters:**


* `area` 区域信息筛选, 默认`AreaType::ALL`. 
* `gender` 性别信息筛选, 默认`GenderType::ALL`. 
* `genre` 风格信息筛选, 默认`GenreType::ALL`.



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**See also:** AreaType, GenderType, GenreType 


**Note:**

异步函数, 返回`Task` 





        

<hr>



### function get\_vip\_info 

_获取当前账号vip信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::get_vip_info (
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `credential` 凭据信息 




        

<hr>



### function hotkey 

_获取搜索热词_ 
```C++
Task< Result < nlohmann::json > > qqmusic::hotkey () 
```





**Returns:**

Json格式的搜索热词




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function keyword\_complete 

_搜索词联想补全_ 
```C++
Task< Result < nlohmann::json > > qqmusic::keyword_complete (
    std::string_view keyword
) 
```





**Parameters:**


* `keyword` 当前搜索词



**Returns:**

带有高亮的补全结果




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function phone\_authorize 

_手机号认证_ 
```C++
qqmusic::Task< qqmusic::Result< utils::Credential > > qqmusic::phone_authorize (
    std::string_view phone,
    std::string_view auth_code,
    std::string_view country_code="86"
) 
```





**Parameters:**


* `phone` 手机号 
* `auth_code` 验证码 
* `country_code` 地区前缀



**Returns:**

`qqmusic::Task<qqmusic::Result< qqmusic::utils::Credential >>` 出错就会返回`Err`




**Note:**

异步函数, 返回`Task`




**Warning:**

该函数仍有问题, 不应使用 





        

<hr>



### function query\_song 

_批量查询歌曲信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::query_song (
    std::span< std::string > mids
) 
```





**Parameters:**


* `mids` 待查询歌曲mid列表



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function query\_song 

_批量查询歌曲信息_ 
```C++
Task< Result < nlohmann::json > > qqmusic::query_song (
    std::span< uint64_t > ids
) 
```





**Parameters:**


* `mids` 待查询歌曲id列表



**Returns:**

包含正确JSON格式结果的`Task< Result <nlohmann::json>>`. 如果产生错误, 返回值包含错误结果和错误类型枚举




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function quick\_search 

_快速搜索_ 
```C++
Task< Result < nlohmann::json > > qqmusic::quick_search (
    std::string_view keyword
) 
```





**Parameters:**


* `keyword` 搜索词



**Returns:**

Json格式搜索结果




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function search\_by\_type 

_按类型搜索_ 
```C++
Task< Result < nlohmann::json > > qqmusic::search_by_type (
    std::string_view keyword,
    SearchType type=SearchType::SONG,
    unsigned num_per_page=10,
    unsigned page=1,
    bool highlight=true
) 
```





**Parameters:**


* `keybord` 搜索词 
* `type` 搜索类型 
* `num_per_page` 每页结果数 
* `page` 页数



**Returns:**

Json格式搜索结果




**Note:**

异步函数, 返回`Task` 





        

<hr>



### function send\_authcode 

_向指定手机号发送验证码_ 
```C++
qqmusic::Task< qqmusic::Result< PhoneLoginResult > > qqmusic::send_authcode (
    std::string_view phone,
    std::string_view country_code="86"
) 
```





**Parameters:**


* `phone` 手机号 
* `country_code` 地区前缀



**Returns:**

`qqmusic::Task<qqmusic::Result< PhoneLoginResult >>` 出错就会返回`Err`




**Note:**

异步函数, 返回`Task`




**Warning:**

该函数仍有问题, 不应使用 





        

<hr>



### function songlist\_add\_songs 

_向歌单内添加歌曲_ 
```C++
Task< Result < bool > > qqmusic::songlist_add_songs (
    uint64_t dirid,
    std::span< uint64_t > song_ids,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `dirid` 歌单ID 
* `song_ids` 待添加歌单列表 
* `credential` 凭据信息



**Returns:**

是否添加成功, 歌曲已存在则返回`false` 





        

<hr>



### function songlist\_create 

_创建歌单_ 
```C++
Task< Result < nlohmann::json > > qqmusic::songlist_create (
    std::string_view dirname,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `dirname` 新的歌单名称 
* `credential` 凭据信息



**Returns:**

创建歌单的基本信息




**Note:**

异步函数, 返回`Task`




**Warning:**

歌单名称不应该含有除了正常字符之外的东西, 例如emoji, 这将导致歌单虽然被正确创建, 但是返回时json解析异常报错 





        

<hr>



### function songlist\_delete 

_删除歌单_ 
```C++
Task< Result < bool > > qqmusic::songlist_delete (
    uint64_t dirid,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `dirid` 歌单ID 
* `credential` 凭据信息



**Returns:**

是否删除成功, 如果不存在这个歌单则返回`false` 





        

<hr>



### function songlist\_delete\_songs 

_删除歌单内指定歌曲_ 
```C++
Task< Result < bool > > qqmusic::songlist_delete_songs (
    uint64_t dirid,
    std::span< uint64_t > song_ids,
    std::optional< utils::Credential > credential=std::nullopt
) 
```





**Parameters:**


* `dirid` 歌单ID 
* `song_ids` 待删除歌单列表 
* `credential` 凭据信息



**Returns:**

是否删除成功, 歌曲不存在则返回`false` 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/album.cc`

