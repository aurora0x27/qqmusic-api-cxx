
# Class Hierarchy

This inheritance list is sorted roughly, but not completely, alphabetically:


* **class** [**qqmusic::BaseMediaFileType**](classqqmusic_1_1BaseMediaFileType.md) _媒体文件类型基类_     
    * **class** [**qqmusic::EncryptedSongFileType**](classqqmusic_1_1EncryptedSongFileType.md) _加密歌曲文件类型_ 
    * **class** [**qqmusic::SongFileType**](classqqmusic_1_1SongFileType.md) _未加密的歌曲文件类枚举_ 
* **class** [**qqmusic::QRCodeLoginEvent**](classqqmusic_1_1QRCodeLoginEvent.md) _二维码登陆事件_ 
* **class** [**qqmusic::TabType**](classqqmusic_1_1TabType.md) _标签类_ 
* **class** [**qqmusic::crypto::Cipher**](classqqmusic_1_1crypto_1_1Cipher.md)     
    * **class** [**qqmusic::crypto::MapCipher**](classqqmusic_1_1crypto_1_1MapCipher.md) 
    * **class** [**qqmusic::crypto::RC4Cipher**](classqqmusic_1_1crypto_1_1RC4Cipher.md) 
* **class** [**qqmusic::crypto::Decoder**](classqqmusic_1_1crypto_1_1Decoder.md) 
* **class** [**qqmusic::crypto::KeyDerive**](classqqmusic_1_1crypto_1_1KeyDerive.md) 
* **class** [**qqmusic::details::Api**](classqqmusic_1_1details_1_1Api.md) _Api类, 简化请求报文组装和返回报文解析过程_ 
* **class** [**qqmusic::utils::AsyncExecutor**](classqqmusic_1_1utils_1_1AsyncExecutor.md) _全局调度器类_ 
* **class** [**qqmusic::utils::CookieJar**](classqqmusic_1_1utils_1_1CookieJar.md) _简易Cookie管理器, 负责cookie的序列化和反序列化, 依赖_ `nlohmann::json` _实现 目前不支持路径嵌套和_`Expires` _和_`HttpOnly` _这样的特性_
* **class** [**qqmusic::utils::Credential**](classqqmusic_1_1utils_1_1Credential.md) _凭据信息类_ 
* **class** [**qqmusic::utils::Exception**](classqqmusic_1_1utils_1_1Exception.md) _全局异常类_ 
* **class** [**qqmusic::utils::PathManager**](classqqmusic_1_1utils_1_1PathManager.md) _返回默认路径_ 
* **class** [**qqmusic::utils::Session**](classqqmusic_1_1utils_1_1Session.md) _上下文_ 
* **class** [**qqmusic::utils::SessionGuard**](classqqmusic_1_1utils_1_1SessionGuard.md) _一个_ `RAII` _风格的Session切换器_
* **class** [**qqmusic::utils::SessionManager**](classqqmusic_1_1utils_1_1SessionManager.md) _全局单例的Session管理器, 负责维护全局上下文栈, 并负责_ `session` _实例的生成_
* **struct** [**PhoneLoginEvent**](structPhoneLoginEvent.md) _手机号登陆事件包装器_ 
* **struct** [**Result**](structResult.md) 
* **struct** [**details::And::impl::Then**](structdetails_1_1And_1_1impl_1_1Then.md) 
* **struct** [**details::And::impl::Then&lt; Ret(Arg)&gt;**](structdetails_1_1And_1_1impl_1_1Then_3_01Ret_07Arg_08_4.md) 
* **struct** [**details::And::impl::Then&lt; Ret(void)&gt;**](structdetails_1_1And_1_1impl_1_1Then_3_01Ret_07void_08_4.md) 
* **struct** [**details::Constructor**](structdetails_1_1Constructor.md) 
* **struct** [**details::Constructor&lt; void, E &gt;**](structdetails_1_1Constructor_3_01void_00_01E_01_4.md) 
* **struct** [**details::Or::impl::Else**](structdetails_1_1Or_1_1impl_1_1Else.md) 
* **struct** [**details::Or::impl::Else&lt; Result&lt; T, F &gt;(Arg)&gt;**](structdetails_1_1Or_1_1impl_1_1Else_3_01Result_3_01T_00_01F_01_4_07Arg_08_4.md) 
* **struct** [**details::Or::impl::Else&lt; Result&lt; T, F &gt;(void)&gt;**](structdetails_1_1Or_1_1impl_1_1Else_3_01Result_3_01T_00_01F_01_4_07void_08_4.md) 
* **struct** [**details::Other::impl::Wise**](structdetails_1_1Other_1_1impl_1_1Wise.md) 
* **struct** [**details::Other::impl::Wise&lt; Ret(Arg)&gt;**](structdetails_1_1Other_1_1impl_1_1Wise_3_01Ret_07Arg_08_4.md) 
* **struct** [**details::ResultErrType**](structdetails_1_1ResultErrType.md) 
* **struct** [**details::ResultErrType&lt; Result&lt; T, E &gt; &gt;**](structdetails_1_1ResultErrType_3_01Result_3_01T_00_01E_01_4_01_4.md) 
* **struct** [**details::ResultOkType**](structdetails_1_1ResultOkType.md) 
* **struct** [**details::ResultOkType&lt; Result&lt; T, E &gt; &gt;**](structdetails_1_1ResultOkType_3_01Result_3_01T_00_01E_01_4_01_4.md) 
* **struct** [**details::Storage**](structdetails_1_1Storage.md) 
* **struct** [**details::Storage&lt; void, E &gt;**](structdetails_1_1Storage_3_01void_00_01E_01_4.md) 
* **struct** [**details::err::impl::Map**](structdetails_1_1err_1_1impl_1_1Map.md) 
* **struct** [**details::err::impl::Map&lt; Ret(Cls::\*)(Arg) const &gt;**](structdetails_1_1err_1_1impl_1_1Map_3_01Ret_07Cls_1_1_5_08_07Arg_08_01const_01_4.md) 
* **struct** [**details::err\_tag**](structdetails_1_1err__tag.md) 
* **struct** [**details::impl::result\_of**](structdetails_1_1impl_1_1result__of.md) 
* **struct** [**details::impl::result\_of&lt; Ret(Args...)&gt;**](structdetails_1_1impl_1_1result__of_3_01Ret_07Args_8_8_8_08_4.md)     
    * **struct** [**details::impl::result\_of&lt; Ret(Cls::\*)(Args...)&gt;**](structdetails_1_1impl_1_1result__of_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_4.md) 
* **struct** [**details::ok::impl::Map**](structdetails_1_1ok_1_1impl_1_1Map.md)     
    * **struct** [**details::ok::Map**](structdetails_1_1ok_1_1Map.md) 
* **struct** [**details::ok::impl::Map&lt; Result&lt; U, E &gt;(Arg)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Result_3_01U_00_01E_01_4_07Arg_08_4.md) 
* **struct** [**details::ok::impl::Map&lt; Result&lt; U, E &gt;(void)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Result_3_01U_00_01E_01_4_07void_08_4.md) 
* **struct** [**details::ok::impl::Map&lt; Ret(Arg)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Ret_07Arg_08_4.md)     
    * **struct** [**details::ok::impl::Map&lt; Ret(Cls::\*)(Arg) const &gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Ret_07Cls_1_1_5_08_07Arg_08_01const_01_4.md) 
    * **struct** [**details::ok::impl::Map&lt; Ret(Cls::\*)(Arg)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Ret_07Cls_1_1_5_08_07Arg_08_4.md) 
* **struct** [**details::ok::impl::Map&lt; Ret(void)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01Ret_07void_08_4.md) 
* **struct** [**details::ok::impl::Map&lt; void(Arg)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01void_07Arg_08_4.md) 
* **struct** [**details::ok::impl::Map&lt; void(void)&gt;**](structdetails_1_1ok_1_1impl_1_1Map_3_01void_07void_08_4.md) 
* **struct** [**details::ok\_tag**](structdetails_1_1ok__tag.md) 
* **struct** [**details::result\_of&lt; Ret(\*)(Args...)&gt;**](structdetails_1_1result__of_3_01Ret_07_5_08_07Args_8_8_8_08_4.md) 
* **struct** [**details::result\_of&lt; Ret(Cls::\*)(Args...) const &gt;**](structdetails_1_1result__of_3_01Ret_07Cls_1_1_5_08_07Args_8_8_8_08_01const_01_4.md) 
* **struct** [**details::void\_t**](structdetails_1_1void__t.md) 
* **struct** [**qqmusic::Lyric**](structqqmusic_1_1Lyric.md) _歌词_ 
* **struct** [**qqmusic::MvUrl**](structqqmusic_1_1MvUrl.md) _Mv url封裝_ 
* **struct** [**qqmusic::PhoneLoginResult**](structqqmusic_1_1PhoneLoginResult.md) 
* **struct** [**qqmusic::QRCode**](structqqmusic_1_1QRCode.md) _二维码信息_ 
* **struct** [**qqmusic::QRCodeLoginResult**](structqqmusic_1_1QRCodeLoginResult.md) _二维码登陆结果封装_ 
* **struct** [**qqmusic::SongInfo**](structqqmusic_1_1SongInfo.md) 
* **struct** [**qqmusic::details::ApiConfig**](structqqmusic_1_1details_1_1ApiConfig.md) _Api相关常量_ 
* **struct** [**qqmusic::details::NetworkContext**](structqqmusic_1_1details_1_1NetworkContext.md) _打包的上下文_ 
* **struct** [**qqmusic::details::RequestParam**](structqqmusic_1_1details_1_1RequestParam.md) _包装请求所需的参数_ 
* **struct** [**qqmusic::utils::Cookie**](structqqmusic_1_1utils_1_1Cookie.md) _代表一条Cookie_ 
* **struct** [**qqmusic::utils::Device**](structqqmusic_1_1utils_1_1Device.md) _设备信息_ 
* **struct** [**qqmusic::utils::OSVersion**](structqqmusic_1_1utils_1_1OSVersion.md) 
* **struct** [**qqmusic::utils::QimeiResult**](structqqmusic_1_1utils_1_1QimeiResult.md) 
* **struct** [**qqmusic::EncryptedSongFileType::CoreData**](structqqmusic_1_1EncryptedSongFileType_1_1CoreData.md) 
* **struct** [**qqmusic::SongFileType::CoreData**](structqqmusic_1_1SongFileType_1_1CoreData.md) 
* **struct** [**qqmusic::TabType::Tab**](structqqmusic_1_1TabType_1_1Tab.md) _标签信息结构_ 
* **struct** [**types::Err**](structtypes_1_1Err.md) 
* **struct** [**types::Ok**](structtypes_1_1Ok.md) 
* **struct** [**types::Ok&lt; void &gt;**](structtypes_1_1Ok_3_01void_01_4.md) 
* **class** **std::vector< uint8_t >**    
    * **class** [**qqmusic::utils::buffer**](classqqmusic_1_1utils_1_1buffer.md) _二进制缓冲区的统一表示_ 
* **class** **std::false_type**    
    * **struct** [**concepts::EqualityComparable**](structconcepts_1_1EqualityComparable.md) 
    * **struct** [**details::IsResult**](structdetails_1_1IsResult.md) 
* **class** **std::true_type**    
    * **struct** [**concepts::EqualityComparable&lt; T, typename std::enable\_if&lt; true, typename details::void\_t&lt; decltype(std::declval&lt; T &gt;()==std::declval&lt; T &gt;())&gt;::type &gt;::type &gt;**](structconcepts_1_1EqualityComparable_3_01T_00_01typename_01std_1_1enable__if_3_01true_00_01typenb4b0200ec94bdc3bb35c4a1595f66806.md) 
    * **struct** [**details::IsResult&lt; Result&lt; T, E &gt; &gt;**](structdetails_1_1IsResult_3_01Result_3_01T_00_01E_01_4_01_4.md) 

