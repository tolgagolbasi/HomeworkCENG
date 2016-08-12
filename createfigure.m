function createfigure(XData1, YData1, XData2, YData2, X1, Y1, X2, Y2)
%CREATEFIGURE(XDATA1,YDATA1,XDATA2,YDATA2,X1,Y1,X2,Y2)
%  XDATA1:  line xdata
%  YDATA1:  line ydata
%  XDATA2:  line xdata
%  YDATA2:  line ydata
%  X1:  vector of x data
%  Y1:  vector of y data
%  X2:  vector of x data
%  Y2:  vector of y data

%  Auto-generated by MATLAB on 12-May-2013 11:56:34

% Create figure
figure1 = figure('Tag','Print CFTOOL to Figure',...
    'Colormap',[0.929411764705882 0.141176470588235 0.149019607843137;0.893765617681719 0.141436340527268 0.174654519028774;0.858321709664156 0.14219894067563 0.199969845480876;0.823082021231061 0.143438756530217 0.224957042153093;0.7880485329603 0.145130273587922 0.249607563999074;0.75322322542974 0.147247977345641 0.273912865972467;0.718608079217246 0.149766353300269 0.297864403026921;0.684205074900687 0.152659886948702 0.321453630116085;0.650016193057928 0.155903063787833 0.344672002193608;0.616043414266836 0.159470369314558 0.367510974213139;0.582288719105278 0.163336289025771 0.389962001128326;0.548754088151119 0.167475308418369 0.412016537892818;0.515441501982228 0.171861912989245 0.433666039460265;0.482352941176471 0.176470588235294 0.454901960784314;0.447320553226747 0.182882373814046 0.475673410744107;0.408894041651553 0.192464015073651 0.495968299266413;0.36815172106715 0.20486855604694 0.515836282876323;0.3261719060898 0.219749040766741 0.535327018098928;0.284032911335764 0.236758513265885 0.55449016145932;0.242813051421303 0.255550017577201 0.57337536948259;0.203590640962678 0.275776597733518 0.592032298693829;0.167443994576151 0.297091297767666 0.610510605618129;0.135451426877982 0.319147161712474 0.62885994678058;0.108691252484433 0.341597233600772 0.647129978706274;0.0882417860117654 0.36409455746539 0.665370357920302;0.0751813420762397 0.386292177339157 0.683630740947755;0.0705882352941176 0.407843137254902 0.701960784313725;0.0730457936740441 0.431511800928367 0.721519643322979;0.080055638799646 0.459565471755541 0.7430259286684;0.0910735256496573 0.490911603289629 0.765868479957254;0.105555209202812 0.524457649083836 0.78943613679681;0.122956444437843 0.559111062691364 0.813117738794332;0.142732986333485 0.593779297665419 0.836302125557088;0.164340589868472 0.627369807559205 0.858378136692345;0.187235010021537 0.658790045925925 0.87873461180737;0.210872001771415 0.686947466318783 0.896760390509428;0.234707320096839 0.710749522290984 0.911844312405786;0.258196719976543 0.729103667395733 0.923375217103713;0.28079595638926 0.740917355186232 0.930741944210473;0.301960784313725 0.745098039215686 0.933333333333333;0.322503220932295 0.745098039215686 0.923587018142822;0.34358970933387 0.745098039215686 0.896227775964063;0.365207643357001 0.745098039215686 0.854075161886222;0.387344416840237 0.745098039215686 0.799948730998464;0.409987423622129 0.745098039215686 0.736668038389952;0.433124057541227 0.745098039215686 0.667052639149852;0.456741712436082 0.745098039215686 0.593922088367329;0.480827782145243 0.745098039215686 0.520095941131547;0.505369660507261 0.745098039215686 0.448393752531669;0.530354741360685 0.745098039215686 0.381635077656863;0.555770418544067 0.745098039215686 0.32263947159629;0.581604085895956 0.745098039215686 0.274226489439118;0.607843137254902 0.745098039215686 0.23921568627451;0.634596597354862 0.74540326827135 0.211930212659109;0.661972188035458 0.746286826064062 0.185015351826023;0.68995534879374 0.747700518532402 0.158773857879118;0.718531519126755 0.749596151614947 0.133508484922256;0.747686138531553 0.751925531250279 0.109521987059302;0.777404646505183 0.754640463376976 0.0871171183941192;0.807672482544694 0.757692753933617 0.0665966330305718;0.838475086147135 0.761034208858782 0.0482632850725233;0.869797896809555 0.764616634091051 0.0324198286238375;0.901626354029002 0.768391835569002 0.0193690177883786;0.933945897302526 0.772311619231216 0.00941360667001012;0.966741966127176 0.77632779101627 0.00285634937259593;1 0.780392156862745 0],...
    'Color',[0.933333333333333 0.933333333333333 0.933333333333333]);

% Create axes
axes1 = axes('Parent',figure1);
%% Uncomment the following line to preserve the X-limits of the axes
% xlim(axes1,[15 45.2]);
%% Uncomment the following line to preserve the Y-limits of the axes
% ylim(axes1,[-0.238 5.0389]);
%% Uncomment the following line to preserve the Z-limits of the axes
% zlim(axes1,[-1 1]);
box(axes1,'on');
grid(axes1,'on');
hold(axes1,'all');

% Create xlabel
xlabel('bit');

% Create ylabel
ylabel('time(s)');

% Create zlabel
zlabel('Z','Visible','off');

% Create line
line(XData1,YData1,'Parent',axes1,'LineWidth',1.5,...
    'Color',[0.0705882352941176 0.407843137254902 0.701960784313725],...
    'DisplayName','untitled fit 1');

% Create line
line(XData2,YData2,'Parent',axes1,'MarkerFaceColor',[0 0 0],...
    'MarkerEdgeColor',[0 0 0],...
    'MarkerSize',3,...
    'Marker','o',...
    'LineStyle','none',...
    'DisplayName','y vs. x');

% Create plot
plot(X1,Y1,'Marker','.','LineStyle','none','Color',[0 0 1],...
    'DisplayName','time vs. bits');

% Create plot
plot(X2,Y2,'Color',[1 0 0],'DisplayName','Pollard Rho');

