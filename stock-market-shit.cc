#include<iostream>
#include<fstream>
#include<cstdlib>
#include<iomanip>
//#include<string>

void shujv();
void hangqing();
void geren();
void gongsi();
void geren_zhuce(); 
void gongsi_zhuce();
void kaishi();
void shoushi();
void shengou(int);
void buydan(int);
void selldan(int);
void cancelbuydan(int);
void cancelselldan(int);
void faxinggupiao(int);
void fenhong(int);
void touzi(int);
void shoucheng(int);
typedef struct person *per;
typedef struct corporation *corp;
typedef struct gupiao *shares;
typedef struct Dan *dan;
struct person{
    int daihao;
    float zijin;
    float djzijin;
    per next;
};
struct corporation{
    int daihao;
    int lt_gupiao;
    int wsg_gupiao;
    float zijin;
    float gujia;
    float ss_gujia;
    corp next;
};
struct gupiao{
    int person;
    int corporation;
    int number;
    shares next;
};
struct Dan{
    int person;
    int corporation;
    int number;
    float gujia;
    dan next;
};
per per_head;
corp corp_head;
shares shares_head;
dan buydan_head;
dan selldan_head;
shares shengou_head;
per find_per_by_daihao(int daihao)
{
    per temp = per_head->next;
    while(temp != NULL)
        if(temp->daihao == daihao)
            return temp;
        else temp = temp->next;
    return NULL;
}
corp find_corp_by_daihao(int daihao)
{
    corp temp = corp_head->next;
    while(temp != NULL)
        if(temp->daihao == daihao)
            return temp;
        else temp = temp->next;
    return NULL;
}
shares find_shengou_by_per_corp(int person, int corporation)
{
    shares temp = shengou_head->next;
    while(temp != NULL)
        if((temp->person == person) && temp->corporation == corporation)
            return temp;
        else temp = temp->next;
    return NULL;
}
shares find_front_shares_by_per_corp(int person, int corporation)
{
    shares temp = shares_head;
    while(temp->next != NULL)
        if((temp->next->person == person) && (temp->next->corporation == corporation))
            return temp;
        else temp = temp->next;
    return temp;
}
float per_money_all_gupiao(int person)
{
    shares temp_shares = shares_head;
    float sum=0;
    while(temp_shares->next != NULL){
        temp_shares = temp_shares->next;
        if(temp_shares->person == person)
            sum += find_corp_by_daihao(temp_shares->corporation)->gujia * temp_shares->number;
    }
    return sum;
}
int the_min(int a, int b)
{
    if(a < b)
        return a;
    return b;
}
void jisuan()
{
    float buydan_num, selldan_num, num1=0, num2=0, num3=0;
    dan temp_dan;
    corp temp_corp = corp_head;
    while(temp_corp->next != NULL){
        temp_corp = temp_corp->next;
        temp_dan = buydan_head;
        while(temp_dan->next != NULL){
            temp_dan = temp_dan->next;
            if(temp_dan->corporation == temp_corp->daihao){
                num2 += (temp_dan->number * temp_dan->gujia);
                num1++;
            }
        }
        temp_dan = selldan_head;
        while(temp_dan->next != NULL){
            temp_dan = temp_dan->next;
            if(temp_dan->corporation == temp_corp->daihao){
                num3 += (temp_dan->number * temp_dan->gujia);
                num1++;
            }
        }
        temp_corp->gujia = (num2 + num3) / num1;
    }
    //计算单
    dan temp_buydan=buydan_head, temp_selldan, temp_delete;
    per temp_per1, temp_per2;
    int min_num;
    float temp_money;
    bool isdelest = false;
    while(temp_buydan->next != NULL){
        temp_selldan=selldan_head;
        while(temp_selldan->next != NULL){
            //找到
            if((temp_buydan->next->corporation == temp_selldan->next->corporation)&&(temp_buydan->next->gujia == temp_selldan->next->gujia)){
                temp_per1 = find_per_by_daihao(temp_buydan->next->person);
                temp_per2 = find_per_by_daihao(temp_selldan->next->person);
                //资金移动
                min_num = the_min(temp_buydan->next->number, temp_selldan->next->number);
                temp_money = min_num * temp_buydan->next->gujia;
                temp_per1->zijin -= temp_money;
                temp_per1->djzijin -= temp_money;
                temp_per2->zijin += temp_money;
                //单的改变
                temp_buydan->next->number -= min_num;
                if(temp_buydan->next->number <= 0){
                    temp_delete = temp_buydan->next;
                    temp_buydan->next = temp_delete->next;
                    free(temp_delete);
                    isdelest = true;
                    break;
                }
                temp_selldan->next->number -= min_num;
                if(temp_selldan->next->number <= 0){
                    temp_delete = temp_selldan->next;
                    temp_selldan->next = temp_delete->next;
                    free(temp_delete);
                }
                else
                    temp_selldan = temp_selldan->next;
            }
            else
                temp_selldan = temp_selldan->next;
        }
        if(!isdelest)
            temp_buydan = temp_buydan->next;
    }
}
/*main menu*/
int main()
{
    //输入个人数据。
    std::fstream per_infile;
    per_infile.open("geren.txt", std::ios::in);
    if(!per_infile){
        std::cerr<<"Open 'geren.txt' error!"<<std::endl;
        exit(1);
    }
    per_head = (per)malloc(sizeof(struct person));
    per_head->next = NULL;
    per per_fol = per_head;
    while(per_infile.peek() != EOF){
        per_fol->next = (per)malloc(sizeof(struct person));
        per_fol = per_fol->next;
        per_infile>>per_fol->daihao;
        per_infile>>per_fol->zijin;
        per_infile>>per_fol->djzijin;
        per_fol->next = NULL;
    }
    per_infile.close();
    //输入公司数据。
    std::fstream corp_infile;
    corp_infile.open("gongsi.txt", std::ios::in);
    if(!corp_infile){
        std::cerr<<"Open 'gongsi.txt' error!"<<std::endl;
        exit(1);
    }
    corp_head = (corp)malloc(sizeof(struct corporation));
    corp_head->next = NULL;
    corp corp_fol = corp_head;
    while(corp_infile.peek() != EOF){
        corp_fol->next = (corp)malloc(sizeof(struct corporation));
        corp_fol = corp_fol->next;
        corp_infile>>corp_fol->daihao;
        corp_infile>>corp_fol->lt_gupiao;
        corp_infile>>corp_fol->wsg_gupiao;
        corp_infile>>corp_fol->zijin;
        corp_infile>>corp_fol->gujia;
        corp_infile>>corp_fol->ss_gujia;
        corp_fol->next = NULL;
    }
    corp_infile.close();
    //输入股票数据
    std::fstream shares_infile;
    shares_infile.open("gupiao.txt", std::ios::in);
    if(!shares_infile){
        std::cerr<<"Open 'gupiao.txt' error!"<<std::endl;
        exit(1);
    }
    shares_head = (shares)malloc(sizeof(struct gupiao));
    shares_head->next = NULL;
    shares shares_fol = shares_head;
    while(shares_infile.peek() != EOF){
        shares_fol->next = (shares)malloc(sizeof(struct gupiao));
        shares_fol = shares_fol->next;
        shares_infile>>shares_fol->person;
        shares_infile>>shares_fol->corporation;
        shares_infile>>shares_fol->number;
        shares_fol->next = NULL;
    }
    shares_infile.close();
    //输入买单数据
    std::fstream buydan_infile;
    buydan_infile.open("buydan.txt", std::ios::in);
    if(!buydan_infile){
        std::cerr<<"Open 'buydan.txt' error!"<<std::endl;
        exit(1);
    }
    buydan_head = (dan)malloc(sizeof(struct Dan));
    buydan_head->next = NULL;
    dan buydan_fol = buydan_head;
    while(buydan_infile.peek() != EOF){
        buydan_fol->next = (dan)malloc(sizeof(struct Dan));
        buydan_fol = buydan_fol->next;
        buydan_infile>>buydan_fol->person;
        buydan_infile>>buydan_fol->corporation;
        buydan_infile>>buydan_fol->number;
        buydan_infile>>buydan_fol->gujia;
        buydan_fol->next = NULL;
    }
    buydan_infile.close();
    //输入卖单数据
    std::fstream selldan_infile;
    selldan_infile.open("selldan.txt", std::ios::in);
    if(!selldan_infile){
        std::cerr<<"Open 'selldan.txt' error!"<<std::endl;
        exit(1);
    }
    selldan_head = (dan)malloc(sizeof(struct Dan));
    selldan_head->next = NULL;
    dan selldan_fol = selldan_head;
    while(selldan_infile.peek() != EOF){
        selldan_fol->next = (dan)malloc(sizeof(struct Dan));
        selldan_fol = selldan_fol->next;
        selldan_infile>>selldan_fol->person;
        selldan_infile>>selldan_fol->corporation;
        selldan_infile>>selldan_fol->number;
        selldan_infile>>selldan_fol->gujia;
        selldan_fol->next = NULL;
    }
    selldan_infile.close();
    //输入申购数据
    std::fstream shengou_infile;
    shengou_infile.open("shengou.txt", std::ios::in);
    if(!shengou_infile){
        std::cerr<<"Open 'shengou.txt' error!"<<std::endl;
        exit(1);
    }
    shengou_head = (shares)malloc(sizeof(struct gupiao));
    shengou_head->next = NULL;
    shares shengou_fol = shengou_head;
    while(shengou_infile.peek() != EOF){
        shengou_fol->next = (shares)malloc(sizeof(struct gupiao));
        shengou_fol = shengou_fol->next;
        shengou_infile>>shengou_fol->person;
        shengou_infile>>shengou_fol->corporation;
        shengou_infile>>shengou_fol->number;
        shengou_fol->next = NULL;
    }
    shengou_infile.close();
    //软件开始运行
    std::cout<<"欢迎使用股市管理软件"<<std::endl<<
                "此软件由【陆李阳】制作，版权由【陆李阳】所有。"<<std::endl<<std::endl<<
                "！！！重要提示！！！"<<std::endl<<
                "请谨慎仔细进行每一步操作"<<std::endl<<std::endl;
    char input_num;
    bool quit = false;
    while(!quit){
        std::cout<<"你想做什么？（键入序号，回车确定。）"<<std::endl;
        std::cout<<"【1】数据 【2】行情 【3】个人 【4】公司 【5】退出（保存数据）"<<std::endl;
        std::cin>>input_num;
        switch(input_num){
        case '1':
            shujv();
            break;
        case '2':
            hangqing();
            break;
        case '3':
            geren();
            break;
        case '4':
            gongsi();
            break;
        case '5':
            quit = true;
            break;
        default:
            std::cout<<"你的输入有错，请重试。"<<std::endl;
        }
    }
    //保存个人数据。
    std::fstream per_outfile;
    per_outfile.open("geren.txt", std::ios::out);
    while(!per_outfile){
        std::cerr<<"Open 'geren.txt' error! 请检查好后按回车重试。"<<std::endl;
        getchar();
        getchar();
    }
    per_fol = per_head;
    per_outfile<<std::setprecision(2);
    while(per_fol->next != NULL){
        per_fol = per_fol->next;
        per_outfile<<per_fol->daihao<<std::endl;
        per_outfile<<setiosflags(std::ios::fixed);
        per_outfile<<per_fol->zijin<<std::endl;
        per_outfile<<per_fol->djzijin;
        per_outfile<<resetiosflags(std::ios::fixed);
        if(per_fol->next != NULL)
            per_outfile<<std::endl;
        else
            break;
    }
    per_outfile.close();
    //保存公司数据。
    std::fstream corp_outfile;
    corp_outfile.open("gongsi.txt", std::ios::out);
    while(!corp_outfile){
        std::cerr<<"Open 'gongsi.txt' error! 请检查好后按回车重试。"<<std::endl;
        getchar();
        getchar();
    }
    corp_fol = corp_head;
    corp_outfile<<std::setprecision(2);
    while(corp_fol->next != NULL){
        corp_fol = corp_fol->next;
        corp_outfile<<corp_fol->daihao<<std::endl;
        corp_outfile<<corp_fol->lt_gupiao<<std::endl;
        corp_outfile<<corp_fol->wsg_gupiao<<std::endl;
        corp_outfile<<setiosflags(std::ios::fixed);
        corp_outfile<<corp_fol->zijin<<std::endl;
        corp_outfile<<corp_fol->gujia<<std::endl;
        corp_outfile<<corp_fol->ss_gujia;
        corp_outfile<<resetiosflags(std::ios::fixed);
        if(corp_fol->next != NULL)
            corp_outfile<<std::endl;
        else
            break;
    }
    corp_outfile.close();
    //保存股票数据。
    std::fstream shares_outfile;
    shares_outfile.open("gupiao.txt", std::ios::out);
    while(!shares_outfile){
        std::cerr<<"Open 'gupiao.txt' error! 请检查好后按回车重试。"<<std::endl;
        getchar();
        getchar();
    }
    shares_fol = shares_head;
    while(shares_fol->next != NULL){
        shares_fol = shares_fol->next;
        shares_outfile<<shares_fol->person<<std::endl;
        shares_outfile<<shares_fol->corporation<<std::endl;
        shares_outfile<<shares_fol->number;
        if(shares_fol->next != NULL)
            shares_outfile<<std::endl;
        else
            break;
    }
    shares_outfile.close();
    //保存买单数据。
    std::fstream buydan_outfile;
    buydan_outfile.open("buydan.txt", std::ios::out);
    while(!buydan_outfile){
        std::cerr<<"Open 'buydan.txt' error! 请检查好后按回车重试。"<<std::endl;
        getchar();
        getchar();
    }
    buydan_fol = buydan_head;
    buydan_outfile<<std::setprecision(2);
    while(buydan_fol->next != NULL){
        buydan_fol = buydan_fol->next;
        buydan_outfile<<buydan_fol->person<<std::endl;
        buydan_outfile<<buydan_fol->corporation<<std::endl;
        buydan_outfile<<buydan_fol->number<<std::endl;
        buydan_outfile<<setiosflags(std::ios::fixed);
        buydan_outfile<<buydan_fol->gujia;
        buydan_outfile<<resetiosflags(std::ios::fixed);
        if(buydan_fol->next != NULL)
            buydan_outfile<<std::endl;
        else
            break;
    }
    buydan_outfile.close();
    //保存卖单数据。
    std::fstream selldan_outfile;
    selldan_outfile.open("selldan.txt", std::ios::out);
    while(!selldan_outfile){
        std::cerr<<"Open 'selldan.txt' error! 请检查好后按回车重试。"<<std::endl;
        getchar();
        getchar();
    }
    selldan_fol = selldan_head;
    selldan_outfile<<std::setprecision(2);
    while(selldan_fol->next != NULL){
        selldan_fol = selldan_fol->next;
        selldan_outfile<<selldan_fol->person<<std::endl;
        selldan_outfile<<selldan_fol->corporation<<std::endl;
        selldan_outfile<<selldan_fol->number<<std::endl;
        selldan_outfile<<setiosflags(std::ios::fixed);
        selldan_outfile<<selldan_fol->gujia;
        selldan_outfile<<resetiosflags(std::ios::fixed);
        if(selldan_fol->next != NULL)
            selldan_outfile<<std::endl;
        else
            break;
    }
    selldan_outfile.close();
    //保存申购数据。
    std::fstream shengou_outfile;
    shengou_outfile.open("shengou.txt", std::ios::out);
    while(!shengou_outfile){
        std::cerr<<"Open 'shengou.txt' error! 请检查好后按回车重试。"<<std::endl;
        getchar();
        getchar();
    }
    shengou_fol = shengou_head;
    while(shengou_fol->next != NULL){
        shengou_fol = shengou_fol->next;
        shengou_outfile<<shengou_fol->person<<std::endl;
        shengou_outfile<<shengou_fol->corporation<<std::endl;
        shengou_outfile<<shengou_fol->number;
        if(shengou_fol->next != NULL)
            shengou_outfile<<std::endl;
        else
            break;
    }
    shengou_outfile.close();
    return 0;
}
/********************
the menu of main menu
********************/
void shujv()
{
    char input_num;
    while(true){
        std::cout<<"******************************************************"<<std::endl<<"你已进入了【数据】选项。"<<std::endl;
        std::cout<<"你想做什么？（键入序号，回车确定。）"<<std::endl;
        std::cout<<"【1】个人注册 【2】公司注册 【3】开市 【4】收市 【5】返回"<<std::endl;
        std::cin>>input_num;
        switch(input_num){
        case '1':
            geren_zhuce();
            break;
        case '2':
            gongsi_zhuce();
            break;
        case '3':
            kaishi();
            break;
        case '4':
            shoushi();
            break;
        case '5':
            std::cout<<"******************************************************"<<std::endl<<"你已返回上一菜单。"<<std::endl;
            return;
        default:
            std::cout<<"你的输入有错，请重试。"<<std::endl;
        }
    }
}
/*the menu of shujv*/
void geren_zhuce()
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【个人注册】选项。"<<std::endl;
    std::cout<<"请输入注册的个人代号，回车确定。（格式为：高一1班请输入 10101 ）"<<std::endl;
    int zhuce_daihao;
    float zhuce_zijin;
    std::cin>>zhuce_daihao;
    if(find_per_by_daihao(zhuce_daihao) != NULL)
        std::cout<<"此人已被注册。"<<std::endl;
    else{
        std::cout<<"请输入注册的初始资金，回车确定。"<<std::endl;
        std::cin>>zhuce_zijin;
        per temp = per_head;
        while(temp->next != NULL)
            temp = temp->next;
        temp->next = (per)malloc(sizeof(struct person));
        temp->next->daihao = zhuce_daihao;
        temp->next->zijin = zhuce_zijin;
        temp->next->djzijin = 0;
        temp->next->next = NULL;
    }
    std::cout<<"OK，你已返回上一菜单。"<<std::endl;
}
void gongsi_zhuce()
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【公司注册】选项。"<<std::endl;
    std::cout<<"请输入注册的公司代号，回车确定。"<<std::endl;
    int zhuce_daihao;
    std::cin>>zhuce_daihao;
    if(find_corp_by_daihao(zhuce_daihao) != NULL)
        std::cout<<"此公司已被注册。"<<std::endl;
    else{
        corp temp = corp_head;
        while(temp->next != NULL)
            temp = temp->next;
        temp->next = (corp)malloc(sizeof(struct corporation));
        temp->next->daihao = zhuce_daihao;
        temp->next->lt_gupiao = 0;
        temp->next->wsg_gupiao = 0;
        temp->next->zijin = 0;
        temp->next->gujia = 10;
        temp->next->ss_gujia = 10;
        temp->next->next = NULL;
    }
    std::cout<<"OK，返回上一层。"<<std::endl;
}
void kaishi()
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【开市】选项。"<<std::endl;
    shares temp_delete;
    per temp_per;
    corp temp_corp;
    while(shengou_head->next != NULL){
        temp_delete = shengou_head->next;
        shengou_head->next = temp_delete->next;
        temp_per = find_per_by_daihao(temp_delete->person);
        temp_corp = find_corp_by_daihao(temp_delete->corporation);
        temp_per->djzijin -= (temp_delete->number * temp_corp->gujia);
        free(temp_delete);
    }
    temp_corp = corp_head;
    while(temp_corp->next != NULL){
        temp_corp = temp_corp->next;
        temp_corp->wsg_gupiao = 0;
    }
    jisuan();
    std::cout<<"OK，返回上一层。"<<std::endl;
}
void shoushi()
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【收市】选项。"<<std::endl;
    jisuan();
    corp temp_corp = corp_head;
    while(temp_corp->next != NULL){
        temp_corp = temp_corp->next;
        temp_corp->ss_gujia = temp_corp->gujia;
    }
    dan temp_delete;
    per temp_per;
    while(buydan_head->next != NULL){
        temp_delete = buydan_head->next;
        buydan_head->next = temp_delete->next;
        temp_per = find_per_by_daihao(temp_delete->person);
        temp_per->djzijin -= temp_delete->number * temp_delete->gujia;
        free(temp_delete);
    }
    while(selldan_head->next != NULL){
        temp_delete = selldan_head->next;
        selldan_head->next = temp_delete->next;
        free(temp_delete);
    }
    std::cout<<"OK，返回上一层。"<<std::endl;
}
/********************
the menu of main menu
********************/
void hangqing()
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【行情】选项。"<<std::endl;
    std::cout<<std::endl<<"代号"<<'\t'<<"价格"<<'\t'<<"涨停"<<'\t'<<"跌停"<<'\t'<<"（键入 q ，回车返回。）"<<std::endl;
    corp temp = corp_head;
    while(temp->next != NULL){
        temp = temp->next;
        std::cout<<temp->daihao<<'\t'<<temp->gujia<<'\t'<<temp->gujia*1.5<<'\t'<<temp->gujia*0.5<<std::endl;
    }
    char input_num;
    while(std::cin>>input_num){
        if (input_num == 'q'){
            std::cout<<"******************************************************"<<std::endl<<"你已返回上一菜单。"<<std::endl;
            return;
        }
        std::cout<<"请听话。-_-"<<std::endl;
    }
}
/********************
the menu of main menu
********************/
void geren()
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【个人】选项。"<<std::endl;
    std::cout<<"请输入个人代号，回车确定。"<<std::endl;
    int input_daihao;
    std::cin>>input_daihao;
    per temp_per;
    if((temp_per=find_per_by_daihao(input_daihao)) == NULL){
        std::cout<<"此人不存在。"<<std::endl;
        return;
    }
    std::cout<<std::endl;
    std::cout<<"代号："<<input_daihao<<std::endl;
    std::cout<<"资产："<<temp_per->zijin + per_money_all_gupiao(input_daihao)<<std::endl;
    std::cout<<"资金："<<temp_per->zijin<<std::endl;
    std::cout<<"冻结资金："<<temp_per->djzijin<<std::endl;
    std::cout<<"持股情况："<<std::endl<<"\t"<<"公司代号"<<"\t"<<"数量"<<std::endl;
    shares chigu = shares_head;
    while(chigu->next != NULL){
        chigu = chigu->next;
        if(chigu->person == input_daihao)
            std::cout<<"\t"<<chigu->corporation<<"\t\t"<<chigu->number<<std::endl;
    }
    std::cout<<"买单："<<std::endl<<"\t"<<"公司代号"<<"\t"<<"价格"<<"\t\t"<<"数量"<<std::endl;
    dan temp_buydan = buydan_head;
    while(temp_buydan->next != NULL){
        temp_buydan = temp_buydan->next;
        if(temp_buydan->person == input_daihao)
            std::cout<<"\t"<<temp_buydan->corporation<<"\t\t"<<temp_buydan->gujia<<"\t\t"<<temp_buydan->number<<std::endl;
    }
    std::cout<<"卖单："<<std::endl<<"\t"<<"公司代号"<<"\t"<<"价格"<<"\t\t"<<"数量"<<std::endl;
    dan temp_selldan = selldan_head;
    while(temp_selldan->next != NULL){
        temp_selldan = temp_selldan->next;
        if(temp_selldan->person == input_daihao)
            std::cout<<"\t"<<temp_selldan->corporation<<"\t\t"<<temp_selldan->gujia<<"\t\t"<<temp_selldan->number<<std::endl;
    }
    std::cout<<std::endl;
    char input_num;
    while(true){
        std::cout<<"【1】申购 【2】买单 【3】卖单 【4】撤全部买单 【5】撤全部卖单 【6】返回"<<std::endl;
        std::cin>>input_num;
        switch(input_num){
        case '1':
            shengou(input_daihao);
            break;
        case '2':
            buydan(input_daihao);
            break;
        case '3':
            selldan(input_daihao);
            break;
        case '4':
            cancelbuydan(input_daihao);
            break;
        case '5':
            cancelselldan(input_daihao);
            break;
        case '6':
            std::cout<<"******************************************************"<<std::endl<<"你已返回上一菜单。"<<std::endl;
            return;
        default:
            std::cout<<"你的输入有错，请重试。"<<std::endl;
        }
    }
}
void shengou(int input_per)
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【申购】选项。"<<std::endl;
    std::cout<<"请输入要申购的公司代号，回车确定。"<<std::endl;
    int input_daihao, input_num;
    std::cin>>input_daihao;
    while(find_shengou_by_per_corp(input_per, input_daihao) != NULL){
        std::cout<<"此公司已申购过，请重试。"<<std::endl;
        std::cin>>input_daihao;
    }
    corp temp_corp = find_corp_by_daihao(input_daihao);
    if(temp_corp->wsg_gupiao == 0)
        std::cout<<"此公司没有股票可被申购"<<std::endl;
    else{
        std::cout<<"此公司未申购股票数为"<<temp_corp->wsg_gupiao<<std::endl<<"请输入要申购的数量，回车确定。"<<std::endl;
        std::cin>>input_num;
        while(input_num > temp_corp->wsg_gupiao){
            std::cout<<"你申购的数量超了。请重试。"<<std::endl;
            std::cin>>input_num;
        }
        float temp_money = temp_corp->gujia * input_num;
        per temp_per = find_per_by_daihao(input_per);
        if(temp_money > (temp_per->zijin - temp_per->djzijin))
            std::cout<<"资金不足。"<<std::endl;
        else{
            //对个人操作
            temp_per->djzijin += temp_money;
            shares temp_shengou = shengou_head;
            while(temp_shengou->next != NULL)
                temp_shengou = temp_shengou->next;
            temp_shengou->next = (shares)malloc(sizeof(struct gupiao));
            temp_shengou = temp_shengou->next;
            temp_shengou->person = input_per;
            temp_shengou->corporation = input_daihao;
            temp_shengou->number = input_num;
            temp_shengou->next = NULL;
            //对公司操作
            temp_corp->wsg_gupiao -= input_num;
            if(temp_corp->wsg_gupiao > 0)
                std::cout<<"申购成功。"<<std::endl;
            else{
                //寻找【申购】中的公司
                temp_shengou = shengou_head;
                while(temp_shengou->next != NULL){
                    //查看每一个是不是
                    //找到了
                    if(temp_shengou->next->corporation == input_daihao){
                        temp_per = find_per_by_daihao(temp_shengou->next->person);
                        temp_money = temp_shengou->next->number * temp_corp->gujia;
                        temp_corp->lt_gupiao += temp_shengou->next->number;
                        //是否在【股票】中存在
                        shares temp_shares = find_front_shares_by_per_corp(temp_shengou->next->person, input_daihao);
                        //有存在，增加，删除【申购】
                        if(temp_shares->next != NULL){
                            temp_shares->next->number += temp_shengou->next->number;
                            shares temp_delete = temp_shengou->next;
                            temp_shengou->next = temp_delete->next;
                                free(temp_delete);
                        }
                        //不存在，创建
                        else{
                            temp_shares->next = temp_shengou->next;
                            temp_shengou->next = temp_shengou->next->next;
                            temp_shares->next->next = NULL;
                        }
                        //资金移动
                        temp_per->zijin -= temp_money;
                        temp_per->djzijin -= temp_money;
                        temp_corp->zijin += temp_money;
                    }
                    //没找到
                    else
                        temp_shengou = temp_shengou->next;
                }
            }
        }
    }
    std::cout<<"OK，返回上一层。"<<std::endl;
}
void buydan(int input_per)
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【买单】选项。"<<std::endl;
    std::cout<<"请输入要买的公司代号，回车确定。"<<std::endl;
    int input_daihao;
    std::cin>>input_daihao;
    std::cout<<"请输入要买的股票数量，回车确定。"<<std::endl;
    int input_num;
    std::cin>>input_num;
    std::cout<<"请输入要买的股票单价，回车确定。"<<std::endl;
    int input_danjia;
    std::cin>>input_danjia;
    //够不够钱
    per temp_per = find_per_by_daihao(input_per);
    corp temp_corp = find_corp_by_daihao(input_daihao);
    if((input_num * input_danjia) > (temp_per->zijin - temp_per->djzijin))
        std::cout<<"可用资金不足。"<<std::endl;
    else if(input_num > temp_corp->lt_gupiao)
        std::cout<<"下单数超过公司总发行量。"<<std::endl;
    else if(input_danjia > (temp_corp->ss_gujia * 1.5))
        std::cout<<"超涨停价。"<<std::endl;
    else if(input_danjia < (temp_corp->ss_gujia * 0.5))
        std::cout<<"低跌停价。"<<std::endl;
    else{
        temp_per->djzijin += input_num * input_danjia;
        dan temp_buydan = buydan_head;
        while(temp_buydan->next != NULL)
            temp_buydan = temp_buydan->next;
        temp_buydan->next = (dan)malloc(sizeof(struct Dan));
        temp_buydan = temp_buydan->next;
        temp_buydan->person = input_per;
        temp_buydan->corporation = input_daihao;
        temp_buydan->number = input_num;
        temp_buydan->gujia = input_danjia;
        temp_buydan->next = NULL;
    }
    std::cout<<"OK，返回上一层。"<<std::endl;
}
void selldan(int input_per)
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【卖单】选项。"<<std::endl;
    std::cout<<"请输入要卖的公司代号，回车确定。"<<std::endl;
    int input_daihao;
    std::cin>>input_daihao;
    std::cout<<"请输入要卖的股票数量，回车确定。"<<std::endl;
    int input_num;
    std::cin>>input_num;
    std::cout<<"请输入要卖的股票单价，回车确定。"<<std::endl;
    int input_danjia;
    std::cin>>input_danjia;
    //数量是否超
    int temp_shu = 0;
    dan temp_selldan = selldan_head;
    while(temp_selldan->next != NULL){
        temp_selldan = temp_selldan->next;
        if((temp_selldan->person == input_per)&&(temp_selldan->corporation == input_daihao))
            temp_shu += temp_selldan->number;
    }
    shares temp_shares = find_front_shares_by_per_corp(input_per, input_daihao);
    //超了
    corp temp_corp = find_corp_by_daihao(input_daihao);
    if(input_num > (temp_shares->next->number - temp_shu))
        std::cout<<"可卖股票不足。"<<std::endl;
    else if(input_danjia > (temp_corp->ss_gujia * 1.5))
        std::cout<<"超涨停价。"<<std::endl;
    else if(input_danjia < (temp_corp->ss_gujia * 0.5))
        std::cout<<"低跌停价。"<<std::endl;
    else{
        temp_selldan->next = (dan)malloc(sizeof(struct Dan));
        temp_selldan = temp_selldan->next;
        temp_selldan->person = input_per;
        temp_selldan->corporation = input_daihao;
        temp_selldan->number = input_num;
        temp_selldan->gujia = input_danjia;
        temp_selldan->next = NULL;
        std::cout<<"OK，返回上一层。"<<std::endl;
    }
}
void cancelbuydan(int input_per)
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【撤全部买单】选项。"<<std::endl;
    dan temp_buydan = buydan_head, temp_delete;
    per temp_per = find_per_by_daihao(input_per);
    while(temp_buydan->next != NULL){
        if(temp_buydan->next->person == input_per){
            temp_per->djzijin -= temp_buydan->next->number * temp_buydan->next->gujia;
            temp_delete = temp_buydan->next;
            temp_buydan->next = temp_delete->next;
            free(temp_delete);
        }
        else
            temp_buydan = temp_buydan->next;
    }
    std::cout<<"OK，返回上一层。"<<std::endl;
}
void cancelselldan(int input_per)
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【撤全部卖单】选项。"<<std::endl;
    dan temp_selldan = selldan_head, temp_delete;
    while(temp_selldan->next != NULL){
        if(temp_selldan->next->person == input_per){
            temp_delete = temp_selldan->next;
            temp_selldan->next = temp_delete->next;
            free(temp_delete);
        }
        else
            temp_selldan = temp_selldan->next;
    }
    std::cout<<"OK，返回上一层。"<<std::endl;
}
/********************
the menu of main menu
********************/
void gongsi()
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【公司】选项。"<<std::endl;
    std::cout<<"请输入公司代号，回车确定。"<<std::endl;
    int input_daihao;
    std::cin>>input_daihao;
    corp temp_corp;
    if((temp_corp=find_corp_by_daihao(input_daihao)) == NULL){
        std::cout<<"此公司不存在。"<<std::endl;
        return;
    }
    std::cout<<std::endl;
    std::cout<<"代号："<<input_daihao<<std::endl;
    //std::cout<<"市值："<<<<std::endl;
    std::cout<<"股价："<<temp_corp->gujia<<std::endl;
    std::cout<<"流通股票量："<<temp_corp->lt_gupiao<<std::endl;
    std::cout<<"资金："<<temp_corp->zijin<<std::endl;
    std::cout<<std::endl;
    char input_num;
    while(true){
        std::cout<<"【1】发行股票 【2】分红 【3】投资 【4】收成 【5】返回"<<std::endl;
        std::cin>>input_num;
        switch(input_num){
        case '1':
            faxinggupiao(input_daihao);
            break;
        case '2':
            fenhong(input_daihao);
            break;
        case '3':
            touzi(input_daihao);
            break;
        case '4':
            shoucheng(input_daihao);
            break;
        case '5':
            std::cout<<"******************************************************"<<std::endl<<"你已返回上一菜单。"<<std::endl;
            return;
        default:
            std::cout<<"你的输入有错，请重试。"<<std::endl;
        }
    }
}
void faxinggupiao(int input_corp)
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【发行股票】选项。"<<std::endl;
    corp temp_corp = find_corp_by_daihao(input_corp);
    if(temp_corp->wsg_gupiao > 0)
        std::cout<<"此公司现在不能发行股票。"<<std::endl;
    else{
    std::cout<<"请输入要发行股票数，回车确定。"<<std::endl;
    int input_fx_gupiao;
    std::cin>>input_fx_gupiao;
    temp_corp->wsg_gupiao = input_fx_gupiao;
    }
    std::cout<<"OK，返回上一层。"<<std::endl;
}
void fenhong(int input_corp)
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【分红】选项。"<<std::endl;
    std::cout<<"请输入要分红钱数，回车确定。"<<std::endl;
    int input_fenhong;
    std::cin>>input_fenhong;
    corp temp_corp = find_corp_by_daihao(input_corp);
    if((input_fenhong * temp_corp->lt_gupiao) > temp_corp->zijin)
        std::cout<<"资金不足。"<<std::endl;
    else{
        temp_corp->zijin -= input_fenhong * temp_corp->lt_gupiao;
        shares temp_shares = shares_head;
        while(temp_shares->next != NULL){
            temp_shares = temp_shares->next;
            if(temp_shares->corporation == input_corp){
                per temp_per = find_per_by_daihao(temp_shares->person);
                temp_per->zijin += input_fenhong * temp_shares->number;
            }
        }
    }
    std::cout<<"OK，返回上一层。"<<std::endl;
}
void touzi(int input_corp)
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【投资】选项。"<<std::endl;
    std::cout<<"请输入投资钱数，回车确定。"<<std::endl;
    int input_touzi;
    std::cin>>input_touzi;
    corp temp_corp = find_corp_by_daihao(input_corp);
    temp_corp->zijin -= input_touzi;
    std::cout<<"OK，返回上一层。"<<std::endl;
}
void shoucheng(int input_corp)
{
    std::cout<<"******************************************************"<<std::endl<<"你已进入了【收成】选项。"<<std::endl;
    std::cout<<"请输入收成钱数，回车确定。"<<std::endl;
    int input_shoucheng;
    std::cin>>input_shoucheng;
    corp temp_corp = find_corp_by_daihao(input_corp);
    temp_corp->zijin += input_shoucheng;
    std::cout<<"OK，返回上一层。"<<std::endl;
}
