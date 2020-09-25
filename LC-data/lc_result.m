clear all
close all
clc

% Constants
Liplusm3permol = 0.05022;
%Liplusm3permol = 1.3e-2;
RT = 9.870E-01;
delta = 1;
%%
load('lcdata.mat')
times(18) = times(18) - 275.455; %data point starts from this time

%Problematic cases:
% 0.01 - 0.001 1.12
% 0.0001 - 0.01  -- 1.06
% 0.01 - 1.00 -- 1.06
% 0.01 - 0.01  -- 1.12


%%
load('lcdata_150.mat')
ps = ps';
cs = cs';
times = times';
aratios = aratios';
arcratios = arcratios';
times(18) = times(18) - 275.455; %data point starts from this time


%%
% Adding 0-0 values Run only once
% appending values for zero
Ws = 2*[ps; 0.0]/delta;
Ks = [cs; 0.0]*2.0*RT/Liplusm3permol;
times = [times; 334.8636505191132]/334.8636505191132;
aratios = [aratios; 0.758275];
arcratios = [arcratios; 1.700950800676068];
aratiosvsX(25,:) = [1.0, 1.0, 0.9898333333333333, 0.983625, 0.95715, 0.9180416666666666, 0.8732142857142857, 0.8333125, 0.7950833333333334, 0.758275, 0.7251363636363637, 0.6940833333333334, 0.6645961538461539, 0.63725, 0.6109333333333333, 0.585421875];
arcratiosvsX(25,:) = [1.0, 1.0, 1.0538477631085024, 1.0745584412271572, 1.1221930009000631, 1.1676907656649087, 1.2646365902454164, 1.3994050093511934, 1.5502919227461645, 1.700950800676068, 1.8446864110969248, 2.0092918278805616, 2.162575177241069, 2.323225641765152, 2.4838167911774813, 2.6315635048587156];
roughnessvalsvsX(25,:) = [0.0, 0.0, 1.0, 2.0, 5.0, 10.0, 16.0, 22.0, 29.0, 37.0, 45.0, 53.0, 62.0, 71.0, 81.0, 91.0];
%%
plotctr(Ws,Ks,times, '$\tilde{t}_{sc}$')

%%
plotctr(Ws,Ks,arcratios, '$\tilde{L}$')
%%
plotctr(Ws,Ks,aratios, '$\tilde{A}$')

%%
nt = 7;
plotctr(Ws,Ks, roughnessvalsvsX(:,nt)/roughnessvalsvsX(25,nt), '$\tilde{RF}$')
%export_fig -m3 roughctr.png
%% Roughness new
ps = Ws;
cs = Ks;
nt = 7;
label = '$\tilde{RF}$';
z = roughnessvalsvsX(:,nt)/roughnessvalsvsX(25,nt);
F1 = scatteredInterpolant(ps(z>=0),cs(z>=0),z(z>=0));
z(z>=0)
ps(ps>=0)
cs(cs>=0)
n=20;
[xq,yq] = meshgrid(logspace(-6, log10(20), n), logspace(-8, log10(39.3), n));
ztq  = F1(xq,yq);

fig = figure;
h = pcolor(xq,yq,ztq);
%h = contourf(xq,yq,ztq);
colorbar;

cmap=hot;
cust=cmap(7:end,:);
colormap(flipud(cust));
shading interp;

%only for tsc
%mycmap = get(fig,'Colormap')
%set(fig,'Colormap',flipud(mycmap))

%set(h, 'EdgeColor', 'none');
%colorbar
%This part only for arc length ratio
hold on;
hp = contour(xq,yq,ztq,[0.5 0.5],'--','LineWidth',4,'Color','k');
%[Cp, hp] = contourf(xq,yq,ztq,[0.6 0.6]);
%contour(ztq,)
%,'--','LineWidth',4,'Color','k'

ax1 = gca;
set(ax1,'Box','on')
xlim([1e-1, 20])
ylim([1e-6, 39.3])
set(ax1,'Xscale','log')
set(ax1,'Yscale','log')
hold on;
plot(5.54e-2, 3.56e-6,'kd', 'MarkerFaceColor','b','MarkerSize',20)

ylabel(ax1,'$\mathbf{Elastic~ Constant}~ \tilde{K}$','Interpreter','latex','FontWeight','bold','FontSize',24);
xlabel(ax1, '$\mathbf{Anchoring~ Energy}~ \tilde{W}$','Interpreter','latex','FontWeight','bold','FontSize',24)
set(gca,'FontName','Arial','FontSize',24,'LineWidth',4)
set(gca,'XTick',[1e-5, 1e-4, 1e-3,1e-2, 1e-1, 1e0, 1e1])
set(gcf,'Color','w','Position', [0, 0, 600, 500])
set(gca, 'Layer', 'top')
annotation('textbox','String', label, 'Interpreter','latex','FontSize',27,'Fontname','Arial','EdgeColor','none','Position',[0.85,0.89,0.1,0.1])



%% Interpolation



F1 = scatteredInterpolant(ps(times>0),cs(times>0),times(times>0));
n=10;
[xq,yq] = meshgrid(logspace(-8, 1., n), logspace(-8, 0., n));
ztq  = F1(xq,yq);

%%
%ps = [0.0, 10.0, 1.0, 10.0, 10.0, 1.0, 0.0001, 1.0, 1e-06, 1.0, 1.0, 1e-06, 1e-06, 0.0001, 1e-06, 1e-06, 0.01, 0.0001, 0.01, 0.01, 0.01, 0.0001, 0.01];
%Ks = [0.0, 0.01, 1e-08, 1.0, 0.0001, 1e-06, 0.0001, 0.0001, 1.0, 0.01, 1.0, 1e-08, 1e-06, 1.0, 0.0001, 0.01, 1e-08, 1e-06, 1e-06, 0.01, 1.0, 0.01, 0.0001];
%times = [402.8415318897672, -2.0, 419.86551672486246, 405.41182957510193, -2.0, 336.40309476357163, 354.22910250728467, 336.4378908702142, 360.0822565998565, 370.13542883810567, 336.43778432631944, 336.4378855590329, 336.403505614448, 336.4379162664688, -2.0, -2.0, 336.39558140928284, -2.0, 329.3632935312509, 329.5654081679891, 327.19569477584855, -2.0];
%Aratio = [0.92065625, -1.0, 0.9694375, 0.9289375, -1.0, 0.764625, 0.8001875, 0.76475, 0.827625, 0.84303125, 0.76475, 0.76475, 0.764625, 0.76475, -1.0, -1.0, 0.76446875, -1.0, 0.77734375, 0.77834375, 0.76259375, -1.0];
n=10;
[xq,yq] = meshgrid(logspace(-4, 1., n), logspace(-4, 0., n));
[xq,yq] = meshgrid(logspace(-8, 1., n), logspace(-8, 0., n));
ztq = griddata(ps(times>0),cs(times>0),times(times>0),xq,yq);
zaq = griddata(ps(aratios>0),cs(aratios>0),aratios(aratios>0),xq,yq);


%%
Aratios10_1 = [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.999375, 0.9842361111111111, 0.9694375, 0.9474431818181818, 0.9141145833333333, 0.8714423076923077, 0.8247098214285714, 0.7777708333333333, 0.73314453125];
Aratios10_1em2 = [1.0, 1.0, 1.0, 1.0, 1.0, 0.9913541666666666, 0.9811160714285714, 0.9646875, 0.9421527777777777, 0.92065625, 0.9064488636363637, 0.8980208333333334, 0.880576923076923, 0.86203125, 0.8419583333333334, 0.81853515625];
Aratios0_0 = [1.0, 1.0, 0.9895833333333334, 0.983046875, 0.955625, 0.9156770833333333, 0.8688839285714286, 0.826796875, 0.7860416666666666, 0.747375, 0.7128125, 0.681171875, 0.6513461538461538, 0.6241741071428571, 0.5985, 0.5738671875];
Xvals = [ 30,  40,  50,  60,  70,  80,  90, 100, 110, 120, 130, 140, 150, 160, 170, 180];
figure; hold on;

plot(Xvals, Aratios0_0,'-go','Linewidth',2)
plot(Xvals, Aratios10_1,'-bo','Linewidth',2)
plot(Xvals, Aratios10_1em2,'-ro','Linewidth',2)
legend({'$\tilde{K}=0$, $\tilde{\chi}=0$', '$\tilde{K}=1$, $\tilde{\chi}=10$', '$\tilde{K}=0.01$, $\tilde{\chi}=10$'},'interpreter','latex','location','best')
legend boxoff;
ax1 = gca;
set(ax1,'Box','on');
xlim([20,inf])
ylabel(ax1,'Area Ratio $\tilde{A}$','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Times New Roman');
xlabel(ax1, '$x$','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Times New Roman')

set(gca,'FontName','Arial','FontSize',24,'LineWidth',4)
set(gcf,'Color','w','Position', [0, 0, 500, 400])

%% Nucleation plot - arc length
load('nucl_AreaArc.mat')
Kval = 1.0*2.0*RT/Liplusm3permol; %393
plot(Xs, 223.799309729/200.0* ArcratiosvsX0,'-ko','Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)
hold on;
plot(Xs, 222.775317488/200 * ArcratiosvsX101,'-bo','Linewidth',2,'MarkerFaceColor','b','MarkerSize',10)

legend({'standard electrolyte', 'LC electrolyte'},'interpreter','latex','location','best')
legend boxoff;
ax1 = gca;
set(ax1,'Box','on');
xlim([40,inf])
ylabel(ax1,'Arc Length Ratio $\tilde{L}$','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial');
xlabel(ax1, '$x_{max}$ ($\mu$m)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial')
set(gca,'XTick',0:40:200)
set(gca,'FontName','Arial','FontSize',24,'LineWidth',4)
set(gcf,'Color','w','Position', [0, 0, 600, 500])
%export_fig -m3 nucl_arc_ratios.pdf

%% Nucleation vs radius
load('nucl_r.mat')
xindex = 13;
%plot(Xs, 223.799309729/200.0* ArcratiosvsX0,'-ko','Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)
%plot(Xs, 223.799309729/200.0* ArcratiosvsX101,'-ko','Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)

rs = [0, 5,10,20];
plot(rs, [2.323225641765152, 205.78584445398434/200.0 * ArcratiosvsX0_r5(13), 211.76725402381348/ 200.0 * ArcratiosvsX0_r10(13), 223.79930972903315/200.0 * ArcratiosvsX0(13) ], '-ko','Linewidth',3,'MarkerFaceColor','k','MarkerSize',15)
hold on;
plot(rs, [1.149864663196856, 204.97056274847716/200.0 * ArcratiosvsX101_r5(13), 210.9519723183063/ 200.0 * ArcratiosvsX101_r10(13), 222.77531748844217/200.0 * ArcratiosvsX101(13) ], '-kd','Linewidth',3,'MarkerFaceColor','k','MarkerSize',15)


arcls = 200 - 2*rs + pi*rs;
plot(rs, arcls/200.0, '-kx','Linewidth',3,'MarkerFaceColor','k','MarkerSize',15)

legend({'standard electrolyte', 'LC electrolyte','Initial state'},'interpreter','latex','location','best')
legend boxoff;
ax1 = gca;
set(ax1,'Box','on');
xlim([0,22])
ylim([1,2.4])
ylabel(ax1,'Arc Length Ratio $\tilde{L}$','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial');
xlabel(ax1, 'Radius $r$ ($\mu$m)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial')
%set(gca,'XTick',0:40:200)
set(gca,'FontName','Arial','FontSize',24,'LineWidth',4)
set(gcf,'Color','w','Position', [0, 0, 600, 500])

%export_fig -m3 'nucl_rdep.pdf'
%%  Nucleation plot - area ratios
Xvals = [ 50,  60,  70,  80,  90, 100, 110, 120, 130, 140, 150, 160, 170, 180];
AratiosX = [0.3283333333333333,
 0.396875,
 0.4288,
 0.45066666666666666,
 0.4644285714285714,
 0.4775625,
 0.4886111111111111,
 0.49955,
 0.5112727272727273,
 0.5240833333333333,
 0.5333076923076923,
 0.5383571428571429,
 0.5381333333333334,
 0.536625];

AratiosX0 = [0.27466666666666667,
 0.32725,
 0.3449,
 0.3471666666666667,
 0.3434285714285714,
 0.336375,
 0.3305,
 0.32365,
 0.3170454545454545,
 0.31025,
 0.30303846153846153,
 0.29678571428571426,
 0.2894,
 0.2811875];

figure
plot(Xvals, AratiosX,'-ko','Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)
hold on;
plot(Xvals, AratiosX0,'-kd','Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)

legend({'$\tilde{K}=39.3$, $\tilde{\chi}=10$', '$\tilde{K}=0$, $\tilde{\chi}=0$'},'interpreter','latex','location','best')
legend boxoff;
ax1 = gca;
set(ax1,'Box','on');
%xlim([20,inf])
ylabel(ax1,'Area Ratio $\tilde{A}$','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial');
xlabel(ax1, '$x$ ($\mu$m)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial')

set(gca,'FontName','Arial','FontSize',24,'LineWidth',4)
set(gcf,'Color','w','Position', [0, 0, 600, 500])

%%  Nucleated -- Xvst and roughness
load('nucl_Xvst.mat')
load('nucl_roughness.mat')
fig = figure;
%set(fig,'defaultAxesColorOrder',[[0 0 0]; [0 0 1]]);
plot(txs(xvals<199), xvals(xvals<199),'--k','Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)
hold on;
plot(txs1(xvals1<199), xvals1(xvals1<199),'k','Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)

legend({ 'standard electrolyte', 'LC electrolyte'},'Interpreter','latex','location','best','AutoUpdate','off')
legend boxoff;
ax1 = gca;
set(ax1,'Box','on');
%xlim([20,inf])
xlabel(ax1,'Time (s)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial');
ylabel(ax1, '$x_{max}$ ($\mu$m)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial')
yyaxis right
set(gca,'YColor','b')
plot(trs(xvals<199), rvals(xvals<199),'--b','Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)
hold on
plot(trs1(xvals1<199), rvals1(xvals1<199),'b','Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)
ylabel(ax1, 'Roughness factor ($\mu$m)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial')
xlim([0,220])
ylim([0,inf])
set(gca,'FontName','Arial','FontSize',24,'LineWidth',4)
set(gcf,'Color','w','Position', [0, 0, 600, 500])
set(gca,'YTick',0:20:200)
yyaxis left
set(gca,'YTick',0:40:200)

%export_fig -m3 evolution_nucl_rough.pdf

%% Plot of roughness and maxX non-nucleated
t0 = [0.6000000000000003,
 6.594999999999904,
 12.594999999999777,
 18.59500000000011,
 24.595000000001047,
 30.595000000001985,
 36.59500000000129,
 42.5950000000001,
 48.594999999998905,
 54.59499999999771,
 60.59499999999652,
 66.59499999999717,
 72.59500000000024,
 78.59500000000331,
 84.59000000000637,
 90.58500000000943,
 96.5850000000125,
 102.58000000001556,
 108.57000000001861,
 114.52500000002159,
 120.4950000000246,
 126.48500000002765,
 132.44500000002438,
 138.43000000001894,
 144.43000000001348,
 150.43000000000802,
 156.43000000000256,
 162.4299999999971,
 168.42999999999165,
 174.4199999999862,
 180.41999999998075,
 186.4149999999753,
 192.40999999996984,
 198.40999999996438,
 204.40999999995893,
 210.40999999995347,
 216.409999999948,
 222.40999999994256,
 228.4099999999371,
 234.40999999993164,
 240.40999999992619,
 246.40999999992073,
 252.40999999991527,
 258.4099999999098,
 264.40999999990436,
 270.4099999998989,
 276.40999999989344,
 282.409999999888,
 288.40499999988253,
 294.4049999998771,
 300.4049999998716,
 306.39999999986617,
 312.3949999998607,
 318.39499999985526,
 324.3949999998498,
 330.39499999984434,
 336.3949999998389,
 342.39499999983343,
 348.31874999982807,
 349.1578906248304];

maxx0 = [21.0,
 22.0,
 23.0,
 24.0,
 25.0,
 26.0,
 28.0,
 29.0,
 30.0,
 31.0,
 32.0,
 33.0,
 35.0,
 36.0,
 37.0,
 38.0,
 40.0,
 41.0,
 42.0,
 43.0,
 45.0,
 46.0,
 48.0,
 49.0,
 50.0,
 52.0,
 53.0,
 54.0,
 56.0,
 57.0,
 59.0,
 60.0,
 62.0,
 63.0,
 65.0,
 66.0,
 68.0,
 70.0,
 72.0,
 74.0,
 76.0,
 79.0,
 81.0,
 84.0,
 87.0,
 90.0,
 93.0,
 97.0,
 101.0,
 105.0,
 109.0,
 115.0,
 121.0,
 127.0,
 134.0,
 143.0,
 153.0,
 167.0,
 199.0,
 200.0];

roughness0 = [0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 1.0,
 0.0,
 0.0,
 0.0,
 1.0,
 1.0,
 2.0,
 1.0,
 1.0,
 2.0,
 2.0,
 1.0,
 2.0,
 2.0,
 2.0,
 2.0,
 3.0,
 3.0,
 4.0,
 3.0,
 4.0,
 5.0,
 6.0,
 7.0,
 8.0,
 10.0,
 11.0,
 13.0,
 15.0,
 16.0,
 18.0,
 20.0,
 23.0,
 26.0,
 29.0,
 33.0,
 38.0,
 42.0,
 48.0,
 56.0,
 64.0,
 78.0,
 110.0,
 111.0];

maxx101 = [21.0,
 22.0,
 23.0,
 24.0,
 25.0,
 26.0,
 28.0,
 29.0,
 30.0,
 31.0,
 32.0,
 33.0,
 35.0,
 36.0,
 37.0,
 38.0,
 39.0,
 41.0,
 42.0,
 43.0,
 44.0,
 46.0,
 47.0,
 48.0,
 50.0,
 51.0,
 52.0,
 54.0,
 55.0,
 56.0,
 58.0,
 59.0,
 60.0,
 62.0,
 63.0,
 65.0,
 66.0,
 68.0,
 69.0,
 71.0,
 72.0,
 74.0,
 75.0,
 77.0,
 79.0,
 80.0,
 82.0,
 84.0,
 85.0,
 87.0,
 89.0,
 91.0,
 93.0,
 95.0,
 97.0,
 99.0,
 101.0,
 103.0,
 105.0,
 108.0,
 110.0,
 113.0,
 115.0,
 118.0,
 121.0,
 124.0,
 128.0,
 132.0,
 136.0,
 143.0,
 151.0,
 170.0];

roughness101 = [0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 0.0,
 1.0,
 1.0,
 1.0,
 1.0,
 1.0,
 2.0,
 2.0,
 2.0,
 3.0,
 3.0,
 4.0,
 4.0,
 4.0,
 5.0,
 6.0,
 7.0,
 9.0,
 10.0,
 14.0,
 19.0,
 35.0];

t101 = [0.6000000000000003,
 6.599999999999904,
 12.599999999999776,
 18.600000000000108,
 24.600000000001046,
 30.600000000001984,
 36.60000000000129,
 42.600000000000094,
 48.5999999999989,
 54.599999999997706,
 60.59999999999651,
 66.59999999999717,
 72.60000000000024,
 78.6000000000033,
 84.60000000000637,
 90.60000000000944,
 96.60000000001251,
 102.60000000001558,
 108.60000000001865,
 114.60000000002172,
 120.60000000002479,
 126.60000000002786,
 132.60000000002438,
 138.60000000001892,
 144.60000000001347,
 150.600000000008,
 156.60000000000255,
 162.5999999999971,
 168.59999999999164,
 174.59999999998618,
 180.59999999998072,
 186.59999999997527,
 192.5999999999698,
 198.59999999996435,
 204.5999999999589,
 210.59999999995344,
 216.59999999994798,
 222.59999999994253,
 228.59999999993707,
 234.5999999999316,
 240.59999999992615,
 246.5999999999207,
 252.59999999991524,
 258.5999999999098,
 264.59999999990436,
 270.5999999998989,
 276.59999999989344,
 282.599999999888,
 288.5999999998825,
 294.59999999987707,
 300.5999999998716,
 306.59499999986616,
 312.5799999998607,
 318.5649999998553,
 324.52499999984985,
 330.4999999998444,
 336.454999999839,
 342.44999999983355,
 348.4249999998281,
 354.3799999998227,
 360.35999999981726,
 366.3499999998118,
 372.3249999998064,
 378.3199999998009,
 384.3149999997955,
 390.31499999979,
 396.31499999978456,
 402.3149999997791,
 408.31499999977365,
 414.3149999997682,
 420.31499999976273,
 426.3149999997573];

fig = figure;
%set(fig,'defaultAxesColorOrder',[[0 0 0]; [0 0 1]]);
plot(t0, maxx0,'--k','Linewidth',3,'MarkerFaceColor','k','MarkerSize',10)
hold on;
plot(t101, maxx101,'k','Linewidth',3,'MarkerFaceColor','k','MarkerSize',10)

legend({ 'standard electrolyte', 'LC electrolyte'},'Interpreter','latex','location','best','AutoUpdate','off')
legend boxoff;
ax1 = gca;
set(ax1,'Box','on');
%xlim([20,inf])
xlabel(ax1,'Time (s)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial');
ylabel(ax1, '$x_{max}$ ($\mu$m)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial')
ylim([0,150])

yyaxis right
set(gca,'YColor','b')
plot(t0, roughness0,'--b','Linewidth',3,'MarkerFaceColor','k','MarkerSize',10)
hold on
plot(t101, roughness101,'b','Linewidth',3,'MarkerFaceColor','k','MarkerSize',10)
ylabel(ax1, 'Roughness factor RF ($\mu$m)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial')
xlim([0,400])
%ylim([0,150])
set(gca,'FontName','Arial','FontSize',24,'LineWidth',4)
set(gcf,'Color','w','Position', [0, 0, 600, 500])

%export_fig -m3 evolution_rough.pdf
%export_fig -m3 evolution_rough_trunc.pdf

%% Plot of arc length and tSC non-nucleated
ArcratiosvsX0 = [1.0,
 1.0538477631085024,
 1.0745584412271572,
 1.1221930009000631,
 1.1676907656649087,
 1.2646365902454164,
 1.3994050093511934,
 1.5502919227461645,
 1.700950800676068,
 1.8446864110969248,
 2.0092918278805616,
 2.162575177241069,
 2.323225641765152,
 2.4838167911774813,
 2.6315635048587156];

ArcratiosvsX101 = [1.0,
 1.0,
 1.0,
 1.0,
 1.0,
 1.0,
 1.0082842712474618,
 1.0289949493661166,
 1.0476345596729062,
 1.0745584412271572,
 1.0869848480983502,
 1.1179522746320396,
 1.149864663196856,
 1.1986092239790829,
 1.249573267141982];

xfs = [ 40,  50,  60,  70,  80,  90, 100, 110, 120, 130, 140, 150, 160, 170, 180];

figure
plot(xfs, ArcratiosvsX0,'-ko','Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)
hold on;
plot(xfs, ArcratiosvsX101,'-bo','Linewidth',2,'MarkerFaceColor','b','MarkerSize',10)

legend({ 'standard electrolyte', 'LC electrolyte'},'interpreter','latex','location','best','AutoUpdate','off')
legend boxoff;
ax1 = gca;
set(ax1,'Box','on');
%xlim([20,inf])
xlabel(ax1,'$x_{max}$ ($\mu$m)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial');
ylabel(ax1, 'Arc Length Ratio $\tilde{L}$','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial')
% yyaxis right
% plot(t0, roughness0,'Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)
% hold on
% plot(t101, roughness101,'--','Linewidth',2,'MarkerFaceColor','k','MarkerSize',10)
%xlim([0,450])

xlim([40,inf])
ylim([1, 2.8])
set(gca,'FontName','Arial','FontSize',24,'LineWidth',4)
set(gcf,'Color','w','Position', [0, 0, 600, 500])
set(gca,'XTick',40:20:200)

%axes('pos',[.1 .6 .2 .2])
%imshow('nucl/K0_x130.png')

%export_fig -m3 evolution_arc.pdf

%% Max driving force
G0 = importdata('nucl0_currdens.mat');
G101 = importdata('nucl101_currdens.mat');
yvals = linspace(0.5,199.5,200);
fig = figure;
%set(fig,'defaultAxesColorOrder',[[0 0 0]; [0 0 1]]);

plot(yvals, G0,'k','Linewidth',3,'MarkerFaceColor','k','MarkerSize',10)
hold on;
plot(yvals, G101,'b','Linewidth',3,'MarkerFaceColor','k','MarkerSize',10)

legend({ 'standard electrolyte', 'LC electrolyte'},'Interpreter','latex','location','best','AutoUpdate','off')
legend boxoff;
ax1 = gca;
set(ax1,'Box','on');
xlim([80,120])
xlabel(ax1,'$y$ ($\mu$m)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial');
ylabel(ax1, 'Growth rate $\mathcal{M}_{\eta}h^{\prime}(\xi)f(\eta)$','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial')
set(gca,'FontName','Arial','FontSize',24,'LineWidth',4)
set(gcf,'Color','w','Position', [0, 0, 600, 500])

%export_fig -m3 nucl_currdens.pdf
%% Other radii
G0 = importdata('nucl0_currdens_r5.mat');
G101 = importdata('nucl101_currdens_r5.mat');
yvals = linspace(0.5,199.5,200);
fig2 = figure;
%set(fig,'defaultAxesColorOrder',[[0 0 0]; [0 0 1]]);

plot(yvals, G0,'k','Linewidth',3,'MarkerFaceColor','k','MarkerSize',10)
hold on;
plot(yvals, G101,'b','Linewidth',3,'MarkerFaceColor','k','MarkerSize',10)

legend({ 'standard electrolyte', 'LC electrolyte'},'Interpreter','latex','location','best','AutoUpdate','off')
legend boxoff;
ax1 = gca;
set(ax1,'Box','on');
xlim([60,140])
xlabel(ax1,'$y$ ($\mu$m)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial');
ylabel(ax1, '$\mathcal{M}_{\eta}h^{\prime}(\xi)f(\eta)$','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial')
set(gca,'FontName','Arial','FontSize',24,'LineWidth',4)
set(gcf,'Color','w','Position', [0, 0, 600, 500])

G0 = importdata('nucl0_currdens_r10.mat');
G101 = importdata('nucl101_currdens_r10.mat');
yvals = linspace(0.5,199.5,200);
fig3 = figure;
%set(fig,'defaultAxesColorOrder',[[0 0 0]; [0 0 1]]);

plot(yvals, G0,'k','Linewidth',3,'MarkerFaceColor','k','MarkerSize',10)
hold on;
plot(yvals, G101,'b','Linewidth',3,'MarkerFaceColor','k','MarkerSize',10)

legend({ 'standard electrolyte', 'LC electrolyte'},'Interpreter','latex','location','best','AutoUpdate','off')
legend boxoff;
ax1 = gca;
set(ax1,'Box','on');
xlim([60,140])
xlabel(ax1,'$y$ ($\mu$m)','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial');
ylabel(ax1, '$\mathcal{M}_{\eta}h^{\prime}(\xi)f(\eta)$','Interpreter','latex','FontWeight','bold','FontSize',24,'Fontname','Arial')
set(gca,'FontName','Arial','FontSize',24,'LineWidth',4)
set(gcf,'Color','w','Position', [0, 0, 600, 500])
