function plotctr(ps, cs, z, label)
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
hp = contour(xq,yq,ztq,[1.5 1.5],'--','LineWidth',4,'Color','k');
%contour(xq,yq,ztq,[0.5 0.5],'--','LineWidth',4,'Color','k');

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



end